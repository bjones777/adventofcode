package day03;

import lombok.*;
import java.io.*;
import java.util.*;

public class Main {

    @Value
    @Builder
    public static class Point {
        int x;
        int y;

        public Point move(int dx, int dy, int width) {
            return Point.builder().x((x+dx) % width).y(y + dy).build();
        }
    }

    static int helper(Set<Point> trees, int width, int height, int dx, int dy) {
        Point currentLoc = Point.builder().x(0).y(0).build();
        int count = 0;
        while(currentLoc.getY() < height) {
            if(trees.contains(currentLoc)) {
                count += 1;
            }
            currentLoc = currentLoc.move(dx, dy, width);
        }   
        return count;
    }

    static int partA(Set<Point> trees, int width, int height) {
        return helper(trees,width,height,3,1);    
    }

    static long partB(Set<Point> trees, int width, int height) {
        long res = 1;
        res *= helper(trees,width,height,1,1);
        res *= helper(trees,width,height,3,1);
        res *= helper(trees,width,height,5,1);
        res *= helper(trees,width,height,7,1);
        res *= helper(trees,width,height,1,2);
        return res;
    }

    public static void main(String[] args) throws IOException {
        Set<Point> trees = new HashSet<>();
        int width = 0;
        int height = 0;
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day03.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                width = Math.max(width, line.length());
                for(int i = 0;i < line.length();++i) {
                    if(line.charAt(i) == '#') {
                        trees.add(Point.builder().x(i).y(height).build());
                    }
                }
                ++height;
            }
        }

        System.out.println(String.format("part A: %d", partA(trees,width,height)));
        System.out.println(String.format("part B: %d", partB(trees,width,height)));
    }
}