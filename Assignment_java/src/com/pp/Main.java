package com.pp;

import java.math.BigDecimal;
import java.math.MathContext;
import java.math.RoundingMode;
import java.util.ArrayList;
import java.util.List;

public class Main {

    static class OreNumbers implements Runnable {
        private String name;
        private int a;
        private int b;
        private int step;
        private List<Integer> nums;

        public OreNumbers(String name, int a, int b, int step) {
            this.name = name;
            this.a = a;
            this.b = b;
            this.step = step;
            this.nums = new ArrayList<>();
        }

        public void run() {
            System.out.println("Thread " + name + " started.");
            System.out.println("Thread " + name + " computing from " + a + " to " + b + " with a step of " + step);
            for (int i = a; i < b; i += step) {
                if (isOreNumber(i)) {
                    nums.add(i);
                }
            }
        }

        public List<Integer> getNums() {
            return nums;
        }
    }

    private static boolean isIntegerValue(BigDecimal bd) {
        return bd.stripTrailingZeros().scale() <= 0;
    }

    private static boolean isOreNumber(int x) {
        int count = 1;
        BigDecimal one = new BigDecimal(1);
        MathContext mc = new MathContext(20, RoundingMode.CEILING);
        BigDecimal invSum = one.divide(new BigDecimal((double)x), mc);
        for (int d = 1; d <= x/2; d++) {
            if (x % d == 0) {
                count++;
                BigDecimal fraction = one.divide(new BigDecimal(d), mc);
                invSum = invSum.add(fraction);
            }
        }
        BigDecimal harmonicMean = new BigDecimal(count).divide(invSum, mc);

        if (isIntegerValue(harmonicMean)) {
            return true;
        }
        return false;
    }

    public static void main(String[] args) {
        int nr_threads = 8;
	    int a = 1;
	    int b = 500000;

        long startTime = System.currentTimeMillis();
        List<Thread> threads = new ArrayList<>();
        List<OreNumbers> oreNumbers = new ArrayList<>();
        for (int i = 0; i < nr_threads; i++) {
            OreNumbers oNum = new OreNumbers("t" + (i + 1), a + i, b, nr_threads);
            oreNumbers.add(oNum);
            Thread t = new Thread(oNum);
            threads.add(t);
            t.start();
        }
        for (Thread t: threads) {
            try {
                t.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        long endTime = System.currentTimeMillis();
        System.out.println("That took " + (endTime - startTime) + " milliseconds");

        for (OreNumbers o: oreNumbers) {
            List<Integer> nums = o.getNums();
            for (int i: nums) {
                System.out.print(i + " ");
            }
            System.out.println();
        }


    }
}
