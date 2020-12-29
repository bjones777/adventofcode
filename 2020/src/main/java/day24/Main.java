package day24;


import common.*;
import java.util.*;
import java.io.*;

import lombok.*;


public class Main {

    enum HexDirection {
        E,
        SE,
        SW,
        W,
        NW,
        NE
    }

    Point2D toOffset(HexDirection hd) {
        switch(hd) {
            case E:
                return new Point2D(2,0);
            case SE:
                return new Point2D(1,-1);
            case NE:
                return new Point2D(1,1);
            case W:
                return new Point2D(-2,0);
            case NW:
                return new Point2D(-1,1);
            case SW:
                return new Point2D(-1,-1);
            default:
                throw new RuntimeException();
        }
    }

    
    List<List<HexDirection>> directions = new ArrayList<>();

    List<HexDirection> parseLine(String line) {
        List<HexDirection> directions = new ArrayList<>();
        for(int i = 0;i < line.length();++i) {
            char ch = line.charAt(i);
            HexDirection hd = HexDirection.E;
            switch(ch) {
                case 'e':
                    break;
                case 's': {
                        ++i;
                        char ch2 = line.charAt(i);
                        if(ch2 != 'e' && ch2 != 'w') {
                            throw new RuntimeException();
                        }
                        hd = ch2 == 'e' ? HexDirection.SE : HexDirection.SW;
                    }
                    break;
                case 'n': {
                        ++i;
                        char ch2 = line.charAt(i);
                        if(ch2 != 'e' && ch2 != 'w') {
                            throw new RuntimeException();
                        }
                        hd = ch2 == 'e' ? HexDirection.NE : HexDirection.NW;
                    }
                    break; 
                case 'w':
                    hd = HexDirection.W;
                    break;
                default:
                    throw new RuntimeException();
            }
            directions.add(hd);
        }
        return directions;
    }

    public Main() {   
    }
    
    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day24.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                directions.add(parseLine(line));            
            }
       }
    }
    
    public String partA() {
        Set<Point2D> hexTiles = new HashSet<>();
        for(List<HexDirection> d : directions) {
            Point2D currentPoint = new Point2D(0,0);
            for(val hd : d) {
                currentPoint = currentPoint.add(toOffset(hd));
            }
            if(hexTiles.contains(currentPoint)) {
                hexTiles.remove(currentPoint);
            }
            else {
                hexTiles.add(currentPoint);
            }
        }
        return Integer.toString(hexTiles.size());
    }

    public Set<Point2D> evolve(Set<Point2D> hexTiles) {
        Map<Point2D, Integer> neighborCount = new HashMap<>();
        for(Point2D p : hexTiles) {
            for(val hd : HexDirection.values()) {
                Point2D np = p.add(toOffset(hd));
                int val = neighborCount.getOrDefault(np, 0);
                val += 1;
                neighborCount.put(np,val);
            }
        }
        Set<Point2D> newSet = new HashSet<>();
        for(val blackTile : hexTiles) {
            int count = neighborCount.getOrDefault(blackTile, 0);
            if(!(count == 0 || count > 2)) {
                newSet.add(blackTile);
            }
        }
        for(val kvp : neighborCount.entrySet()) {
            if(hexTiles.contains(kvp.getKey())) {
                continue;
            }
            if(kvp.getValue() == 2) {
                newSet.add(kvp.getKey());
            }
        }
        return newSet;
    }

    public String partB() {
        Set<Point2D> hexTiles = new HashSet<>();
        for(List<HexDirection> d : directions) {
            Point2D currentPoint = new Point2D(0,0);
            for(val hd : d) {
                currentPoint = currentPoint.add(toOffset(hd));
            }
            if(hexTiles.contains(currentPoint)) {
                hexTiles.remove(currentPoint);
            }
            else {
                hexTiles.add(currentPoint);
            }
        }
        for(int i = 0;i < 100;++i) {
            hexTiles = evolve(hexTiles);
        }
        return Integer.toString(hexTiles.size());    
     }
     
    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}