program bina
   implicit none

   integer, PARAMETER :: N=100*100, NT=1000
   integer :: i
   real, DIMENSION(100,100) :: temp

   open(1, file='temp.txt')
   open(2, file='temp.bin', access='direct', form='unformatted', status='unknown', recl=N*4)

   do i=1, NT
      read(1, *) temp
      write(2, rec=i) temp
   enddo

end program bina