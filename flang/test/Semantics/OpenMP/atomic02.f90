! REQUIRES: openmp_runtime

! RUN: %python %S/../test_errors.py %s %flang_fc1 %openmp_flags

! OpenMP Atomic construct
! section 2.17.7
! operator is one of +, *, -, /, .AND., .OR., .EQV., or .NEQV

program OmpAtomic
   use omp_lib
   CHARACTER c*3, d*3
   LOGICAL l, m, n

   a = 1
   b = 2
   c = 'foo'
   d = 'bar'
   m = .TRUE.
   n = .FALSE.
   !$omp parallel num_threads(4)

   !$omp atomic
   a = a + (4*2)
   !$omp atomic
   a = a*(b + 1)
   !$omp atomic
   a = a - 3
   !$omp atomic
   a = a/(b + 1)
   !$omp atomic
   !ERROR: Invalid or missing operator in atomic update statement
   a = a**4
   !$omp atomic 
   !ERROR: Expected scalar variable on the LHS of atomic update assignment statement
   !ERROR: Invalid or missing operator in atomic update statement
   !ERROR: Expected scalar expression on the RHS of atomic update assignment statement
   c = d 
   !$omp atomic
   !ERROR: Atomic update statement should be of form `l = l operator expr` OR `l = expr operator l`
   !ERROR: Invalid or missing operator in atomic update statement
   l = a .LT. b
   !$omp atomic
   !ERROR: Atomic update statement should be of form `l = l operator expr` OR `l = expr operator l`
   !ERROR: Invalid or missing operator in atomic update statement
   l = a .LE. b
   !$omp atomic
   !ERROR: Atomic update statement should be of form `l = l operator expr` OR `l = expr operator l`
   !ERROR: Invalid or missing operator in atomic update statement
   l = a .EQ. b
   !$omp atomic
   !ERROR: Atomic update statement should be of form `l = l operator expr` OR `l = expr operator l`
   !ERROR: Invalid or missing operator in atomic update statement
   l = a .NE. b
   !$omp atomic
   !ERROR: Atomic update statement should be of form `l = l operator expr` OR `l = expr operator l`
   !ERROR: Invalid or missing operator in atomic update statement
   l = a .GE. b
   !$omp atomic
   !ERROR: Atomic update statement should be of form `l = l operator expr` OR `l = expr operator l`
   !ERROR: Invalid or missing operator in atomic update statement
   l = a .GT. b
   !$omp atomic
   m = m .AND. n
   !$omp atomic
   m = m .OR. n
   !$omp atomic
   m = m .EQV. n
   !$omp atomic
   m = m .NEQV. n
   !$omp atomic update
   a = a + (4*2)
   !$omp atomic update
   a = a*(b + 1)
   !$omp atomic update
   a = a - 3
   !$omp atomic update
   a = a/(b + 1)
   !$omp atomic update
   !ERROR: Invalid or missing operator in atomic update statement
   a = a**4
   !$omp atomic update
   !ERROR: Expected scalar variable on the LHS of atomic update assignment statement
   !ERROR: Invalid or missing operator in atomic update statement
   !ERROR: Expected scalar expression on the RHS of atomic update assignment statement
   c = c//d
   !$omp atomic update
   !ERROR: Atomic update statement should be of form `l = l operator expr` OR `l = expr operator l`
   !ERROR: Invalid or missing operator in atomic update statement
   l = a .LT. b
   !$omp atomic update
   !ERROR: Atomic update statement should be of form `l = l operator expr` OR `l = expr operator l`
   !ERROR: Invalid or missing operator in atomic update statement
   l = a .LE. b
   !$omp atomic update
   !ERROR: Atomic update statement should be of form `l = l operator expr` OR `l = expr operator l`
   !ERROR: Invalid or missing operator in atomic update statement
   l = a .EQ. b
   !$omp atomic update
   !ERROR: Atomic update statement should be of form `l = l operator expr` OR `l = expr operator l`
   !ERROR: Invalid or missing operator in atomic update statement
   l = a .GE. b
   !$omp atomic update
   !ERROR: Atomic update statement should be of form `l = l operator expr` OR `l = expr operator l`
   !ERROR: Invalid or missing operator in atomic update statement
   l = a .GT. b
   !$omp atomic update
   m = m .AND. n
   !$omp atomic update
   m = m .OR. n
   !$omp atomic update
   m = m .EQV. n
   !$omp atomic update
   m = m .NEQV. n
   !$omp end parallel
end program OmpAtomic
