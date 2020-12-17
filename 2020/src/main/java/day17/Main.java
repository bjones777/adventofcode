package day17;

import common.*;

import java.util.*;
import java.io.*;

import lombok.*;


public class Main {

    Set<Point4D> initialPoints4D = new HashSet<>();
    Set<Point4D> activePoints4D = new HashSet<>();
    
    Set<Point3D> initialPoints = new HashSet<>(); 
    Set<Point3D> activePoints = new HashSet<>();
    public Main() {   
    }

    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day17.txt")))) {
            int y = 0;
            for(String line = br.readLine();line != null;line = br.readLine()) {
                for(int x = 0;x < line.length();++x) {
                    if(line.charAt(x) == '#') {
                        initialPoints.add(new Point3D(x,y,0));
                        initialPoints4D.add(new Point4D(x,y,0,0));
                    }
                }
                y++;
            }
       }
    }

    Set<Point3D> step() {
        Map<Point3D,Integer> nc = new HashMap<>();
        Set<Point3D> newActive = new HashSet<>();
        for(Point3D activePoint : activePoints) {
            for(int dx = -1;dx <= 1;++dx) {
                for(int dy = -1;dy <= 1;++dy) {
                    for(int dz = -1;dz <= 1;++dz) {
                        if(dx == 0 && dy == 0 && dz == 0) continue;
                        val newPoint = new Point3D(activePoint.getX()+dx, activePoint.getY() + dy, activePoint.getZ() + dz);
                        int thisNc = nc.getOrDefault(newPoint, 0);
                        nc.put(newPoint, thisNc + 1);
                    }
                }
            }
        }  
        for(val ncEntry : nc.entrySet()) {
            /*
            If a cube is active and exactly 2 or 3 of its neighbors are also active, the cube remains active. Otherwise, the cube becomes inactive.
            If a cube is inactive but exactly 3 of its neighbors are active, the cube becomes active. Otherwise, the cube remains inactive.
            */
            if(activePoints.contains(ncEntry.getKey())) {
                if(ncEntry.getValue() == 2 || ncEntry.getValue() == 3) {
                    newActive.add(ncEntry.getKey());
                }
            }
            else {
                if(ncEntry.getValue() == 3) {
                    newActive.add(ncEntry.getKey());
                }
            }
        }
        return newActive;
    }

    public String partA() {
        activePoints = new HashSet<>();
        activePoints.addAll(initialPoints);
        for(int i = 0;i < 6;++i) {
            System.out.println(activePoints.size());
            activePoints = step();
        }
        return Integer.toString(activePoints.size());
    }

    Set<Point4D> step2() {
        Map<Point4D,Integer> nc = new HashMap<>();
        Set<Point4D> newActive = new HashSet<>();
        for(Point4D activePoint : activePoints4D) {
            for(int dx = -1;dx <= 1;++dx) {
                for(int dy = -1;dy <= 1;++dy) {
                    for(int dz = -1;dz <= 1;++dz) {
                        for(int dw = -1;dw <= 1;++dw) { 
                            if(dx == 0 && dy == 0 && dz == 0 && dw == 0) continue;
                            val newPoint = new Point4D(activePoint.getX()+dx, activePoint.getY() + dy, activePoint.getZ() + dz, activePoint.getW() + dw);
                            int thisNc = nc.getOrDefault(newPoint, 0);
                            nc.put(newPoint, thisNc + 1);
                        }
                    }
                }
            }
        }  
        for(val ncEntry : nc.entrySet()) {
            /*
            If a cube is active and exactly 2 or 3 of its neighbors are also active, the cube remains active. Otherwise, the cube becomes inactive.
            If a cube is inactive but exactly 3 of its neighbors are active, the cube becomes active. Otherwise, the cube remains inactive.
            */
            if(activePoints4D.contains(ncEntry.getKey())) {
                if(ncEntry.getValue() == 2 || ncEntry.getValue() == 3) {
                    newActive.add(ncEntry.getKey());
                }
            }
            else {
                if(ncEntry.getValue() == 3) {
                    newActive.add(ncEntry.getKey());
                }
            }
        }
        return newActive;
    }

    public String partB() {
        activePoints4D = new HashSet<>();
        activePoints4D.addAll(initialPoints4D);
        for(int i = 0;i < 6;++i) {
            System.out.println(activePoints4D.size());
            activePoints4D = step2();
        }
        return Integer.toString(activePoints4D.size());
 
     }
     
    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}