;;;; 	Copyright (C) 1999, 2000, 2001, 2003, 2006, 2009 Free Software Foundation, Inc.
;;;;
;;;; This library is free software; you can redistribute it and/or
;;;; modify it under the terms of the GNU Lesser General Public
;;;; License as published by the Free Software Foundation; either
;;;; version 3 of the License, or (at your option) any later version.
;;;; 
;;;; This library is distributed in the hope that it will be useful,
;;;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;;;; Lesser General Public License for more details.
;;;; 
;;;; You should have received a copy of the GNU Lesser General Public
;;;; License along with this library; if not, write to the Free Software
;;;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
;;;;


;; There are circularities here; you can't import (oop goops compile)
;; before (oop goops). So when compiling, make sure that things are
;; kosher.
(eval-when (compile) (resolve-module '(oop goops)))

(define-module (oop goops dispatch)
  :use-module (oop goops)
  :use-module (oop goops util)
  :use-module (oop goops compile)
  :export (memoize-method!)
  :no-backtrace
  )

;;;
;;; This file implements method memoization.  It will finally be
;;; implemented on C level in order to obtain fast generic function
;;; application also during the first pass through the code.
;;;

;;;
;;; Constants
;;;

(define hashsets 8)
(define hashset-index 6)

(define hash-threshold 3)
(define initial-hash-size 4) ;must be a power of 2 and >= hash-threshold

(define initial-hash-size-1 (- initial-hash-size 1))

(define the-list-of-no-method '(no-method))

;;;
;;; Method cache
;;;

;; (#@dispatch args N-SPECIALIZED #((TYPE1 ... . CMETHOD) ...) GF)
;; (#@dispatch args N-SPECIALIZED HASHSET MASK
;;             #((TYPE1 ... . CMETHOD) ...)
;;             GF)

;;; Representation

;; non-hashed form

(define method-cache-entries cadddr)

(define (set-method-cache-entries! mcache entries)
  (set-car! (cdddr mcache) entries))

(define (method-cache-n-methods exp)
  (n-cache-methods (method-cache-entries exp)))

(define (method-cache-methods exp)
  (cache-methods (method-cache-entries exp)))

;; hashed form

(define (set-hashed-method-cache-hashset! exp hashset)
  (set-car! (cdddr exp) hashset))

(define (set-hashed-method-cache-mask! exp mask)
  (set-car! (cddddr exp) mask))

(define (hashed-method-cache-entries exp)
  (list-ref exp 5))

(define (set-hashed-method-cache-entries! exp entries)
  (set-car! (list-cdr-ref exp 5) entries))

;; either form

(define (method-cache-generic-function exp)
  (list-ref exp (if (method-cache-hashed? exp) 6 4)))

;;; Predicates

(define (method-cache-hashed? x)
  (integer? (cadddr x)))

(define max-non-hashed-index (- hash-threshold 2))

(define (passed-hash-threshold? exp)
  (and (> (vector-length (method-cache-entries exp)) max-non-hashed-index)
       (struct? (car (vector-ref (method-cache-entries exp)
				 max-non-hashed-index)))))

;;; Converting a method cache to hashed form

(define (method-cache->hashed! exp)
  (set-cdr! (cddr exp) (cons 0 (cons initial-hash-size-1 (cdddr exp))))
  exp)

;;;
;;; Cache entries
;;;

(define (n-cache-methods entries)
  (do ((i (- (vector-length entries) 1) (- i 1)))
      ((or (< i 0) (struct? (car (vector-ref entries i))))
       (+ i 1))))

(define (cache-methods entries)
  (do ((i (- (vector-length entries) 1) (- i 1))
       (methods '() (let ((entry (vector-ref entries i)))
		      (if (or (not (pair? entry)) (struct? (car entry)))
			  (cons entry methods)
			  methods))))
      ((< i 0) methods)))

;;;
;;; Method insertion
;;;

(define (method-cache-insert! exp entry)
  (let* ((entries (method-cache-entries exp))
	 (n (n-cache-methods entries)))
    (if (>= n (vector-length entries))
	;; grow cache
	(let ((new-entries (make-vector (* 2 (vector-length entries))
					the-list-of-no-method)))
	  (do ((i 0 (+ i 1)))
	      ((= i n))
	    (vector-set! new-entries i (vector-ref entries i)))
	  (vector-set! new-entries n entry)
	  (set-method-cache-entries! exp new-entries))
	(vector-set! entries n entry))))

(define (hashed-method-cache-insert! exp entry)
  (let* ((cache (hashed-method-cache-entries exp))
	 (size (vector-length cache)))
    (let* ((entries (cons entry (cache-methods cache)))
	   (size (if (<= (length entries) size)
		     size
		     ;; larger size required
		     (let ((new-size (* 2 size)))
		       (set-hashed-method-cache-mask! exp (- new-size 1))
		       new-size)))
	   (min-misses size)
	   (best #f))
      (do ((hashset 0 (+ 1 hashset)))
	  ((= hashset hashsets))
	(let* ((test-cache (make-vector size the-list-of-no-method))
	       (misses (cache-try-hash! min-misses hashset test-cache entries)))
	  (cond ((zero? misses)
		 (set! min-misses 0)
		 (set! best hashset)
		 (set! cache test-cache)
		 (set! hashset (- hashsets 1)))
		((< misses min-misses)
		 (set! min-misses misses)
		 (set! best hashset)
		 (set! cache test-cache)))))
      (set-hashed-method-cache-hashset! exp best)
      (set-hashed-method-cache-entries! exp cache))))

;;;
;;; Caching
;;;

(define (cache-hashval hashset entry)
  (let ((hashset-index (+ hashset-index hashset)))
    (do ((sum 0)
	 (classes entry (cdr classes)))
	((not (and (pair? classes) (struct? (car classes))))
         sum)
      (set! sum (+ sum (struct-ref (car classes) hashset-index))))))

(define (cache-try-hash! min-misses hashset cache entries)
  (let ((mask (- (vector-length cache) 1)))
    (let outer ((in entries) (max-misses 0))
      (if (null? in)
          max-misses
          (let inner ((i (logand mask (cache-hashval hashset (car in))))
                      (misses 0))
            (cond
             ((and (pair? (vector-ref cache i))
                   (eq? (car (vector-ref cache i)) 'no-method))
              (vector-set! cache i (car in))
              (outer (cdr in) (if (> misses max-misses) misses max-misses)))
             (else
              (let ((misses (+ 1 misses)))
                (if (>= misses min-misses)
                    misses ;; this is a return, yo.
                    (inner (logand mask (+ i 1)) misses))))))))))

;;;
;;; Memoization
;;;

;; Backward compatibility
(define (lookup-create-cmethod gf args)
  (no-applicable-method (car args) (cadr args)))

(define (memoize-method! gf args exp)
  (if (not (slot-ref gf 'used-by))
      (slot-set! gf 'used-by '()))
  (let ((applicable ((if (eq? gf compute-applicable-methods)
			 %compute-applicable-methods
			 compute-applicable-methods)
		     gf args)))
    (cond (applicable
	   ;; *fixme* dispatch.scm needs rewriting Since the current
	   ;; code mutates the method cache, we have to work on a
	   ;; copy.  Otherwise we might disturb another thread
	   ;; currently dispatching on the cache.  (No need to copy
	   ;; the vector.)
	   (let* ((new (list-copy exp))
		  (res
		   (cond ((method-cache-hashed? new)
			  (method-cache-install! hashed-method-cache-insert!
						 new args applicable))
			 ((passed-hash-threshold? new)
			  (method-cache-install! hashed-method-cache-insert!
						 (method-cache->hashed! new)
						 args
						 applicable))
			 (else
			  (method-cache-install! method-cache-insert!
						 new args applicable)))))
	     (set-cdr! (cdr exp) (cddr new))
	     res))
	  ((null? args)
	   (lookup-create-cmethod no-applicable-method (list gf '())))
	  (else
	   ;; Mutate arglist to fit no-applicable-method
	   (set-cdr! args (list (cons (car args) (cdr args))))
	   (set-car! args gf)
	   (lookup-create-cmethod no-applicable-method args)))))

(set-procedure-property! memoize-method! 'system-procedure #t)

(define method-cache-install!
  (letrec ((first-n
	    (lambda (ls n)
	      (if (or (zero? n) (null? ls))
		  '()
		  (cons (car ls) (first-n (cdr ls) (- n 1)))))))
    (lambda (insert! exp args applicable)
      (let* ((specializers (method-specializers (car applicable)))
	     (n-specializers
	      (if (list? specializers)
		  (length specializers)
		  (+ 1 (slot-ref (method-cache-generic-function exp)
				 'n-specialized)))))
	(let* ((types (map class-of (first-n args n-specializers)))
	       (cmethod (compute-cmethod applicable types)))
	  (insert! exp (append types cmethod)) ; entry = types + cmethod
	  cmethod))))) ; cmethod
