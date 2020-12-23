package day23;

import java.util.*;
import java.io.*;
import java.math.BigInteger;

import lombok.*;


public class Main {

    public static class Game {
        int currentMove = 0;
        int currentCup = 0;
        List<Integer> cups = new LinkedList<>();
        public Game(List<Integer> inCups) {
            cups.addAll(inCups);
        }

        void doMove() {
            ++currentMove;
            /*
            System.out.println(String.format("-- move %d --", currentMove));
            {
                val sb = new StringBuilder();
                for(int i = 0;i < cups.size();++i) {
                    if(i != 0) {
                        sb.append(" ");
                    }
                    if(i == currentCup) {
                        sb.append("(");
                    }
                    sb.append(cups.get(i));
                    if(i == currentCup) {
                        sb.append(")");
                    }
                }    
                System.out.println(String.format("Cups: %s",sb.toString()));
            }
            */
            
            List<Integer> cupsToPickUp = new ArrayList<>();

            int origIndex = currentCup;
            int origLabel = cups.get(currentCup);
            
            int removeIndex = (currentCup + 1) % cups.size();
                
            for(int nextCups = 0;nextCups < 3;++nextCups) {
                if(removeIndex >= cups.size()) {
                    removeIndex = 0;
                }
                cupsToPickUp.add(cups.get(removeIndex));
                cups.remove(removeIndex);
            }
            //String pickUp = cupsToPickUp.stream().map(c -> Integer.toString(c)).collect(Collectors.joining(", "));
            //System.out.println(String.format("Picked Up: %s", pickUp));
            Map<Integer,Integer> labelToIndex = new HashMap<>();
            for(int i = 0;i < cups.size();++i) {
                labelToIndex.put(cups.get(i), i);
            }
            int currentLabel = origLabel - 1;
            while(!labelToIndex.containsKey(currentLabel)) {
                --currentLabel;
                if(currentLabel < 0) {
                    currentLabel = 9;
                }
            }
            //System.out.println(String.format("Destination: %d", currentLabel));
            int destIndex = labelToIndex.get(currentLabel);
            for(int i = 0; i< cupsToPickUp.size();++i) {
                cups.add(destIndex+1+i, cupsToPickUp.get(i));
            }
            currentCup = (currentCup + 1) % cups.size();
            int currentIndex = cups.indexOf(origLabel);
            while(currentIndex != origIndex) {
                if(currentIndex < origIndex) {
                    int lastLabel = cups.get(cups.size()-1);
                    cups.remove(cups.size()-1);
                    cups.add(0,lastLabel);
                    ++currentIndex;
                }
                else {
                    int lastLabel = cups.get(0);
                    cups.remove(0);
                    cups.add(cups.size(),lastLabel);
                    --currentIndex;
                }
                
            }
            //System.out.println();
        }

        String doNMoves(int n) {
            for(int i = 0;i < n;++i) {
                doMove();
            }
            val sb = new StringBuilder();
            int oneIndex = cups.indexOf(1);
            for(int i = 0;i < cups.size()-1;++i) {
                sb.append(cups.get((oneIndex+1+i) % cups.size()));
            }
            return sb.toString();
        }
    }

    List<Integer> cups = new ArrayList<>();
    public Main() {   
    }
    
    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day23.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                for(int i = 0;i < line.length();++i) {
                    String cup = line.substring(i, i+1);
                    cups.add(Integer.parseInt(cup));
                }
            }
       }
    }
    
    public String partA() {
        val g = new Game(cups);
        return g.doNMoves(100);
    }

    public static class Cup {
        public Cup nextCup;
        public Cup prevCup;
        public final int label;

        public Cup(int inLabel) {
            label = inLabel;
        }

        public Cup removeAfter() {
            Cup nextCupRet = nextCup;
            nextCup = nextCup.nextCup;
            nextCup.nextCup.prevCup = this;
            nextCupRet.nextCup = null;
            nextCupRet.prevCup = null;
            return nextCupRet;
        }

        public void insertAfter(Cup c) {
            c.prevCup = this;
            c.nextCup = nextCup;
            nextCup.prevCup = c;
            nextCup = c;
        }

        public boolean isInList() {
            return nextCup != null;
        }
        
    }

    public static class Game2 {
        int currentMove = 0;
        
        public static final int NUM_CUPS = 1000000;
        List<Cup> cups = new ArrayList<>();
        Cup currentCup;
        Map<Integer,Integer> labelToIndexMap = new HashMap<>();

        public Game2(List<Integer> inCups) {
            int highest = -1;
            for(int i = 0;i < NUM_CUPS;++i) {
                int label = highest;
                if(i < inCups.size()) {
                    label = inCups.get(i);
                    highest = Math.max(highest,label+1);
                    labelToIndexMap.put(label, i);
                }
                else {
                    ++highest;
                }
                val cup = new Cup(label); 
                cups.add(cup);
                if(i > 0) {
                    cup.prevCup = cups.get(i-1);
                    cup.prevCup.nextCup = cup;
                }
            }
            val lastCup = cups.get(NUM_CUPS-1);
            cups.get(0).prevCup = lastCup;
            lastCup.nextCup = cups.get(0);
            currentCup = cups.get(0);
        }

        public int getIndexForLabel(int label) {
            if(labelToIndexMap.containsKey(label)) {
                return labelToIndexMap.get(label);
            }
            return label - 1;
        }

        public boolean isLabelInList(int label) {
            int index = getIndexForLabel(label);
            Cup c = cups.get(index);
            return c.isInList();
        }

        public void insertAfterLabel(int label, List<Cup> pickedUpCups) {
            int index = getIndexForLabel(label);
            Cup dc = cups.get(index);
            for(int i = pickedUpCups.size()-1;i >= 0;--i) {
                Cup c = pickedUpCups.get(i);
                dc.insertAfter(c);
            }
        }

        void doMove() {
            ++currentMove;
            List<Cup> cupsToPickUp = new ArrayList<>();

            for(int nextCups = 0;nextCups < 3;++nextCups) {
                cupsToPickUp.add(currentCup.removeAfter());
            }
            
            int destLabel = currentCup.label-1;
            if(destLabel <= 0) {
                destLabel = cups.size();
            }
            while(!isLabelInList(destLabel)) {
                destLabel -= 1;
                if(destLabel <= 0) {
                    destLabel = cups.size();
                }
            }
            insertAfterLabel(destLabel, cupsToPickUp);
            currentCup = currentCup.nextCup;
        }

        BigInteger getAns() {
            int index = getIndexForLabel(1);
            Cup c = cups.get(index);
            c = c.nextCup;
            BigInteger l0 = new BigInteger(Integer.toString(c.label));
            c = c.nextCup;
            BigInteger l1 = new BigInteger(Integer.toString(c.label));
            return l0.multiply(l1);
        }

        String getOldAns() {
            val sb = new StringBuilder();
            int index = getIndexForLabel(1);
            Cup fc = cups.get(index);
            for(Cup c = fc.nextCup;c != fc;c = c.nextCup) {
                sb.append(c.label);
            }
            return sb.toString();           
        }

        void doNMoves(int n) {
            for(int i = 0;i < n;++i) {
                doMove();
            }
        }
    }
   
    public String partB() {
        val g2 = new Game2(cups);
        g2.doNMoves(10000000);
        return g2.getAns().toString();    
     }
     
    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}