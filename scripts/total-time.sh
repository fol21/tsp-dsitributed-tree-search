echo ==== UK12 ======
echo "*** Pthreads ***"
for i in 1 2 4 8
do
    cat "./logs/UK12/log_uk12_pth_$i.txt" | grep "Total calculation time:"
done
echo "*** MPI ***"
for i in 1 2 4 8
do
    cat "./logs/UK12/log_uk12_mpi_$i.txt" | grep "Total calculation time:"
done

echo ==== P01 ======
echo "*** Pthreads ***"
for i in 1 2 4 8
do
    cat "./logs/P01/log_p01_pth_$i.txt" | grep "Total calculation time:"
done
echo "*** MPI ***"
for i in 1 2 4 8
do
    cat "./logs/P01/log_p01_mpi_$i.txt" | grep "Total calculation time:"
done

