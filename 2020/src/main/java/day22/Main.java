package day22;

import java.util.*;
import java.util.regex.*;
import java.util.stream.Collectors;
import java.io.*;
import java.math.BigInteger;

import lombok.*;


public class Main {

    Pattern playerPattern = Pattern.compile("Player ([0-9]+):");

    public static class Player {
        int playerNum;
        List<Integer> cards;

        public Player(int num, List<Integer> inCards) {
            playerNum = num;
            cards = inCards;
        }

        public int getCardToPlay() {
            return cards.remove(0);
        }

        public int getNumCards() {
            return cards.size();
        }

        public boolean hasLost() {
            return cards.isEmpty();
        }

        public void addCards(Integer c) {
            cards.add(c);
        }

        public BigInteger getScore() {
            BigInteger sum = BigInteger.ZERO;
            int size = cards.size();
            for(int i = 0;i < size;++i) {
                BigInteger multiplier = new BigInteger(Integer.toString(size-i));
                sum = sum.add(multiplier.multiply(new BigInteger(Integer.toString(cards.get(i)))));
            }
            return sum;
        }

        public Player clone() {
            List<Integer> newCards = new LinkedList<>();
            newCards.addAll(cards);
            return new Player(playerNum, newCards);
        }

        public String getRoundState() {
            return Integer.toString(playerNum) 
                + cards.stream().map(c -> Integer.toString(c)).collect(Collectors.joining(","));
        }

        public Player cloneWithNCards(int nCards) {
            List<Integer> newCards = new LinkedList<>();
            for(int i = 0;i < nCards;++i) {
                newCards.add(cards.get(i));
            }
            return new Player(playerNum, newCards);
        }
    }

    List<Player> origPlayers = new ArrayList<>();
    List<Player> players;

    List<Player> clonePlayers() {
        return clonePlayers(origPlayers);
    }

    public static List<Player> clonePlayers(List<Player> inPlayers) {
        List<Player> ps = new ArrayList<>();
        for(val p : inPlayers) {
            ps.add(p.clone());
        }
        return ps;
    }
    
    public Main() {   
    }

    public Player readPlayer(BufferedReader r) throws IOException {
        String line = r.readLine();
        val m = playerPattern.matcher(line);
        if(!m.matches()) {
            throw new RuntimeException();
        }
        int playerNum = Integer.parseInt(m.group(1));
        List<Integer> cards = new LinkedList<>();
        line = r.readLine();
        while(line != null && !line.isBlank()) {
            cards.add(Integer.parseInt(line));
            line = r.readLine();
        }
        return new Player(playerNum, cards);
    }

    
    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day22.txt")))) {
            origPlayers.add(readPlayer(br));
            origPlayers.add(readPlayer(br));
       }
    }

    public boolean doRound() {
        if(players.stream().anyMatch(p -> p.hasLost())) {
            return false;
        }
        int maxCard = -1;
        int maxCardIndex = -1;
        List<Integer> cards = new ArrayList<>();
        for(int pi = 0;pi < players.size();++pi) {
            val p = players.get(pi);
            cards.add(p.getCardToPlay());
            if(maxCardIndex == -1 || maxCard < cards.get(cards.size()-1)) {
                maxCardIndex = pi;
                maxCard = cards.get(cards.size()-1);
            }
        }
        cards.sort((a,b) -> b.compareTo(a));
        final int playerIndex = maxCardIndex;
        cards.stream().forEachOrdered(c -> players.get(playerIndex).addCards(c));
        if(players.stream().anyMatch(p -> p.hasLost())) {
            return false;
        }
        return true;
    }

    public String partA() {
        players = clonePlayers();
        while(doRound()) {
        }
        Player winner = players.stream().filter(p -> !p.hasLost()).findFirst().get();
        return winner.getScore().toString();
    }

    /*
    Recursive Combat still starts by splitting the cards into two decks (you offer to play with the same starting decks as before - it's only fair). Then, the game consists of a series of rounds with a few changes:

    Before either player deals a card, if there was a previous round in this game that had exactly the same cards in the same order in the same players' decks, the game instantly ends in a win for player 1. Previous rounds from other games are not considered. (This prevents infinite games of Recursive Combat, which everyone agrees is a bad idea.)
    Otherwise, this round's cards must be in a new configuration; the players begin the round by each drawing the top card of their deck as normal.
    If both players have at least as many cards remaining in their deck as the value of the card they just drew, the winner of the round is determined by playing a new game of Recursive Combat (see below).
    Otherwise, at least one player must not have enough cards left in their deck to recurse; the winner of the round is the player with the higher-value card.
    */
    @Value 
    public static class RoundState {
        String state;

        public RoundState(List<Player> players) {
            state = players.stream().map(p -> p.getRoundState()).collect(Collectors.joining("|"));
        }
    }

    public static class Game {
        public int round = 1;
        List<Player> players;
        Set<RoundState> roundStates = new HashSet<>();
        int winnerIndex = -1;

        public Game(List<Player> inPlayers) {
            players = clonePlayers(inPlayers);
        }

        public boolean doRound() {
            val roundState = new RoundState(players);
            if(roundStates.contains(roundState)) {
                winnerIndex = 0;
                return false;
            }
            roundStates.add(roundState);
            List<Integer> cards = new ArrayList<>();
            for(int pi = 0;pi < players.size();++pi) {
                val p = players.get(pi);
                cards.add(p.getCardToPlay());
            }
            boolean canRecurse = true;
            for(int ci = 0;ci < cards.size();++ci) {
                int c = cards.get(ci);
                val p = players.get(ci);
                if(c > p.getNumCards()) {
                    canRecurse = false;
                    break;
                }
            }
            if(canRecurse) {
                int winningIndex = doRecursiveCombat(cards);
                int otherIndex = winningIndex == 0 ? 1 : 0;
                players.get(winningIndex).addCards(cards.get(winningIndex));
                players.get(winningIndex).addCards(cards.get(otherIndex));
            }
            else {
                int maxCard = cards.stream().max(Integer::compareTo).get();
                int winningIndex = cards.indexOf(maxCard);
                cards.sort((a,b) -> b.compareTo(a));
                cards.stream().forEachOrdered(c -> players.get(winningIndex).addCards(c));                
            }
            if(players.stream().anyMatch(p -> p.hasLost())) {
                if(players.get(0).hasLost()) {
                    winnerIndex = 1;
                }
                else {
                    winnerIndex = 0;
                }
                return false;
            }
            return true;
        }

        public int doRecursiveCombat(List<Integer> cards) {
            List<Player> newPlayers = new ArrayList<>();
            for(int i = 0;i < cards.size();++i) {
                newPlayers.add(players.get(i).cloneWithNCards(cards.get(i)));
            }
            Game g = new Game(newPlayers);
            while(g.doRound()) {
            }
            return g.winnerIndex;
        }
    }

    
    public String partB() {
        Game g = new Game(clonePlayers());
        while(g.doRound()) {
        }
        int winnerIndex = g.winnerIndex;
        return g.players.get(winnerIndex).getScore().toString();
        
     }
     
    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}