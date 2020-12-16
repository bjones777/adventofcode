package day15;

import java.util.*;
import java.io.*;

import lombok.*;


public class Main {

    //int[] input = { 0,3,6 };
    int[] input = { 13,0,10,12,1,5,8 };

    public Main() {   
    }

    public void readInput() throws IOException {
        
    }

    public String partA() {
        Map<Integer,Integer> lastNumberTurn = new HashMap<>();
        int lastSpoken = -1;
        int turnSpokenBefore = -1;
        for(int turn = 0; turn < input.length;++turn) {
            lastSpoken = input[turn];
            turnSpokenBefore = lastNumberTurn.getOrDefault(lastSpoken, -1);
            lastNumberTurn.put(lastSpoken, turn);
            System.out.println(String.format("%d %d",turn+1,lastSpoken));
        }
        int nextSpoken = 0;
        for(int turn = input.length;turn < 2020;++turn) {
            if(turnSpokenBefore == -1) {
                nextSpoken = 0;
            }
            else {
                nextSpoken = turn-turnSpokenBefore -1;
            }
            turnSpokenBefore = lastNumberTurn.getOrDefault(nextSpoken, -1);
            lastNumberTurn.put(nextSpoken, turn);
            lastSpoken = nextSpoken;
            System.out.println(String.format("%d %d",turn+1,lastSpoken));
        }
        return Integer.toString(lastSpoken);
    }



    public String partB() {
        Map<Integer,Integer> lastNumberTurn = new HashMap<>();
        int lastSpoken = -1;
        int turnSpokenBefore = -1;
        for(int turn = 0; turn < input.length;++turn) {
            lastSpoken = input[turn];
            turnSpokenBefore = lastNumberTurn.getOrDefault(lastSpoken, -1);
            lastNumberTurn.put(lastSpoken, turn);
            //System.out.println(String.format("%d %d",turn+1,lastSpoken));
        }
        int nextSpoken = 0;
        for(int turn = input.length;turn < 30000000;++turn) {
            if(turnSpokenBefore == -1) {
                nextSpoken = 0;
            }
            else {
                nextSpoken = turn-turnSpokenBefore -1;
            }
            turnSpokenBefore = lastNumberTurn.getOrDefault(nextSpoken, -1);
            lastNumberTurn.put(nextSpoken, turn);
            lastSpoken = nextSpoken;
            //System.out.println(String.format("%d %d",turn+1,lastSpoken));
        }
        return Integer.toString(lastSpoken);
     }

    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}