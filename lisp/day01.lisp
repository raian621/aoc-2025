(if (not (eq (length *args*) 1))
  (error "Usage <program> <solution-part>, exiting")
)
(setq *solution-part* (nth 0 *args*))
(if (eq (position *solution-part* '("1" "2") :test #'string=) nil)
  (error "Invalid solution part")
)

(defun read-lines-from-stream (stream)
(loop for line = (read-line stream nil) until (eq line nil)
  collect line
))

(defun direction-to-displacement (direction)
  (setq magnitude (parse-integer (subseq direction 1 nil)))
  (cond 
    ((eq (char direction 0) #\L) (- magnitude))
    ((eq (char direction 0) #\R) (+ magnitude))
  )
)

(defun find-num-zeros (displacements count-passes)
  (setq zeros 0)
  (setq position 50)
  (loop for displacement in displacements do
    (setq distance-from-zero (if (< displacement 0)
      (mod (- position) 100)
      position
    ))
    (setq total_distance (+ distance-from-zero (abs displacement)))
    (setq position (mod (+ position displacement) 100))
    (setq zeros 
      (if count-passes
        ; Count how many times we've passed or landed on zero:
        (+ zeros (floor total_distance 100))
        ; Count how many times we've landed on zero:
        (+ zeros (if (eq position 0) 1 0))
    ))
  )
  zeros
)

(defun solution1 (displacements) (find-num-zeros displacements nil))
(defun solution2 (displacements) (find-num-zeros displacements t))

(setq *input* (map 'list #'direction-to-displacement
  (read-lines-from-stream *standard-input*)
))
(format t "~D~%"
  (cond
    ((string= *solution-part* "1") (solution1 *input*))
    ((string= *solution-part* "2") (solution2 *input*))
  )
)