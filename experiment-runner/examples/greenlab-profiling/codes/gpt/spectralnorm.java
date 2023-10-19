//This is GPT generate the same function as spectralnorm.java

//input eg. COMMAND LINE: javac SpectralNorm.java
//COMMAND LINE: java SpectralNorm 100  

public class spectralnorm {
    public static void main(String[] args) {
        int n = 100;
        if (args.length > 0) {
            n = Integer.parseInt(args[0]);
        }

        System.out.printf("%.9f\n", new spectralnorm().approximate(n));
    }

    double approximate(int n) {
        double[] u = new double[n];
        for (int i = 0; i < n; i++) {
            u[i] = 1.0;
        }

        double[] v = new double[n];
        for (int i = 0; i < n; i++) {
            v[i] = 0.0;
        }

        for (int i = 0; i < 10; i++) {
            multiplyAtAv(n, u, v);
            multiplyAtAv(n, v, u);
        }

        double vBv = 0.0;
        double vv = 0.0;

        for (int i = 0; i < n; i++) {
            vBv += u[i] * v[i];
            vv += v[i] * v[i];
        }

        return Math.sqrt(vBv / vv);
    }

    /* return element i,j of the infinite matrix A */
    double A(int i, int j) {
        return 1.0 / ((i + j) * (i + j + 1) / 2 + i + 1);
    }

    /* multiply vector v by matrix A */
    void multiplyAv(int n, double[] v, double[] Av) {
        for (int i = 0; i < n; i++) {
            Av[i] = 0;
            for (int j = 0; j < n; j++) {
                Av[i] += A(i, j) * v[j];
            }
        }
    }

    /* multiply vector v by matrix A transposed */
    void multiplyAtv(int n, double[] v, double[] Atv) {
        for (int i = 0; i < n; i++) {
            Atv[i] = 0;
            for (int j = 0; j < n; j++) {
                Atv[i] += A(j, i) * v[j];
            }
        }
    }

    /* multiply vector v by matrix A and then by matrix A transposed */
    void multiplyAtAv(int n, double[] v, double[] AtAv) {
        double[] u = new double[n];
        multiplyAv(n, v, u);
        multiplyAtv(n, u, AtAv);
    }
}
