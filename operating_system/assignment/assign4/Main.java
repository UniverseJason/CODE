import static java.lang.System.exit;

public class Main{
    public static void main(String[] args) {

        // check the number of arguments
        if (args.length != 1) {
            System.out.println("ERROR: invalid number of arguments");
            exit(1);
        }

        int n = Integer.parseInt(args[0]);

        // initialize A with random numbers with the range [1.0, 1000.0]
        double[] A = new double[n];
        for (int i = 0; i < n; i++) {
            A[i] = Math.random() * 1000.0;
        }

        double[] B = new double[n];


        /* -------------------- ONE THREAD CASE -------------------- */
        double elapsed = 0.0;
        // copy A into B
        System.arraycopy(A, 0, B, 0, n);

        // start time
        long ts_begin = System.nanoTime();

        // create a thread thB to sort array B
        SelectionSort srt = new SelectionSort(B);
        Thread thB = new Thread(srt);
        thB.start();

        // wait for the thread to finish
        try {
            thB.join();
        }
        catch (InterruptedException e) {
            System.out.println("ERROR: thread interrupted");
            exit(1);
        }

        // end time
        long ts_end = System.nanoTime();
        elapsed = (ts_end - ts_begin) / 1000000000.0;
        System.out.println("Sorting is done in " + elapsed + " ms when one thread is used");

        /* -------------------- TWO THREAD CASE -------------------- */
        elapsed = 0.0;
        ts_begin = 0;
        ts_end = 0;

        double[] Afirsthalf = new double[n/2];
        double[] Asecondhalf = new double[n/2];

        // copy A into Afirsthalf and Asecondhalf
        System.arraycopy(A, 0, Afirsthalf, 0, n/2);
        System.arraycopy(A, n/2, Asecondhalf, 0, n/2);

        // start time
        ts_begin = System.nanoTime();

        // create thA1 sortThread to sort Afirsthalf
        SelectionSort srt1 = new SelectionSort(Afirsthalf);
        Thread thA1 = new Thread(srt1);
        thA1.start();

        // create thA2 sortThread to sort Asecondhalf
        SelectionSort srt2 = new SelectionSort(Asecondhalf);
        Thread thA2 = new Thread(srt2);
        thA2.start();

        // create thM to merge the two sorted arrays
        MergeThread mrg = new MergeThread(Afirsthalf, Asecondhalf, n/2);
        Thread thM = new Thread(mrg);
        thM.start();

        // wait for the thread to finish
        try {
            thA1.join();
            thA2.join();
            thM.join();
        } catch (InterruptedException e) {
            System.out.println("ERROR: thread interrupted");
            exit(1);
        }

        // end time
        ts_end = System.nanoTime();
        elapsed = (ts_end - ts_begin) / 1000000000.0;
        System.out.println("Sorting is done in " + elapsed + " ms when two threads are used");
    }
}

// this class use selection sort to sort an array of doubles
class SelectionSort implements Runnable{
    double [] arr;
    int n, i, j, min_idx;

    public SelectionSort(double [] arr) {
        this.arr = arr;
        n = arr.length;
    }

    public void sort() {
        for (i = 0; i < n; i++) {
            min_idx = i;
            for (j = i+1; j < n; j++) {
                if (arr[j] < arr[min_idx]) {
                    min_idx = j;
                }
            }
            swap(i, min_idx);
        }
    }

    public void swap(int i, int j) {
        double temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    public double[] getArr() {
        return arr;
    }

    @Override
    public void run() {
        sort();
    }
}

// this class will merge two sorted arrays into one
class MergeThread implements Runnable{
    double [] arr1;
    double [] arr2;
    double [] final_arr;
    int n;

    public MergeThread(double [] arr1, double [] arr2, int n) {
        this.arr1 = arr1;
        this.arr2 = arr2;
        this.n = n;
        final_arr = new double[arr1.length + arr2.length];
    }

    // merge two sorted arrays into one
    public void merge() {
        int i=0, j=0, k=0;
        while (i < n && j < n) {
            if (arr1[i] < arr2[j]) {
                final_arr[k++] = arr1[i++];
            } else {
                final_arr[k++] = arr2[j++];
            }
        }
    }

    public double[] getFinalArr() {
        return final_arr;
    }

    @Override
    public void run() {
        merge();
    }
}