package day18;

import java.util.*;
import java.io.*;
import java.math.BigInteger;

import lombok.*;

public class Main {

    List<String> lines = new ArrayList<>();
    Tokenizer tokenizer;
    public Main() {   
    }

    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day18.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                lines.add(line);
            }
       }
    }

    public static enum TokenType {
        NUMBER,
        PLUS,
        MULTIPLY,
        OPEN_PAREN,
        CLOSE_PAREN,
        EOL
    }

    @Value
    public static class Token {
        TokenType type;
        String text;

        public BigInteger getValue() {
            if(type != TokenType.NUMBER) {
                throw new IllegalStateException();
            }
            return new BigInteger(text);
        }
    }

    public static class Tokenizer {
        int index;
        String input;
        Stack<Token> tokenStack = new Stack<>();

        public Tokenizer(String inInput) {
            index = 0;
            input = inInput;
        }

        public void push(Token t) {
            tokenStack.push(t);
        }

        public Token getNextToken() {
            if(!tokenStack.isEmpty()) {
                return tokenStack.pop();
            }
            for(;index < input.length();++index) {
                char ch = input.charAt(index);
                if(Character.isWhitespace(ch)) {
                    continue;
                }
                if(ch == '(') {
                    ++index;
                    return new Token(TokenType.OPEN_PAREN,"(");
                }
                if(ch == ')') {
                    ++index;
                    return new Token(TokenType.CLOSE_PAREN,")");
                }
                if(ch == '+') {
                    ++index;
                    return new Token(TokenType.PLUS,"+");
                }
                if(ch == '*') {
                    ++index;
                    return new Token(TokenType.MULTIPLY,"*");
                }
                if(Character.isDigit(ch)) {
                    val sb = new StringBuilder();
                    ++index;
                    sb.append(ch);
                    while(index < input.length() && Character.isDigit(input.charAt(index))) {
                        sb.append(input.charAt(index));
                    }
                    return new Token(TokenType.NUMBER, sb.toString());
                }                
            }
            return new Token(TokenType.EOL, "");
        }
    }

    public void consume(TokenType t) {
        Token token = tokenizer.getNextToken();
        if(token.getType() != t) {
            throw new IllegalArgumentException();
        }
    }

    public BigInteger parseExpression(boolean endParen) {
        Token t = tokenizer.getNextToken();
        if(t.getType() == TokenType.EOL) {
            return BigInteger.ZERO;
        }
        
        BigInteger lhs = parseIdentifier(t);
        for(t = tokenizer.getNextToken();t.getType() != TokenType.EOL;t = tokenizer.getNextToken()) {
            if(t.getType() == TokenType.PLUS) {
                lhs = lhs.add(parseIdentifier(tokenizer.getNextToken())); 
            }
            else if(t.getType() == TokenType.MULTIPLY) {
                lhs = lhs.multiply(parseIdentifier(tokenizer.getNextToken()));
            }
            else if(endParen && t.getType() == TokenType.CLOSE_PAREN) {
                break;
            }
            else {
                throw new IllegalArgumentException();
            }
        }
        return lhs;
    }

    public BigInteger parseIdentifier(Token t) {
        if(t.getType() == TokenType.NUMBER) {
            return t.getValue();
        }
        if(t.getType() == TokenType.OPEN_PAREN) {
            return parseExpression(true);
        }
        throw new IllegalArgumentException();
    }
    
    public BigInteger evaluateLine(String line) {
        tokenizer = new Tokenizer(line);
        return parseExpression(false);
    }
   
    public String partA() {
        BigInteger sum = BigInteger.ZERO;
        for(String line : lines) {
            sum = sum.add(evaluateLine(line));
        }
        return sum.toString();
    }

    public BigInteger parseIdentifier2(Token t) {
        if(t.getType() == TokenType.NUMBER) {
            BigInteger v = t.getValue();
            
            Token t2 = tokenizer.getNextToken();
            while(t2.getType() == TokenType.PLUS) {
                v = v.add(parseIdentifier2(tokenizer.getNextToken()));
                t2 = tokenizer.getNextToken();
            }
            tokenizer.push(t2);
            return v;
        }
        if(t.getType() == TokenType.OPEN_PAREN) {
            BigInteger v = parseExpression2(true);
            
            Token t2 = tokenizer.getNextToken();
            while(t2.getType() == TokenType.PLUS) {
                v = v.add(parseIdentifier2(tokenizer.getNextToken()));
                t2 = tokenizer.getNextToken();
            }
            tokenizer.push(t2);
            return v;
        }
        throw new IllegalArgumentException();
    }

    public BigInteger parseExpression2(boolean endParen) {
        Token t = tokenizer.getNextToken();
        if(t.getType() == TokenType.EOL) {
            return BigInteger.ZERO;
        }
        
        BigInteger lhs = parseIdentifier2(t);
        for(t = tokenizer.getNextToken();t.getType() != TokenType.EOL;t = tokenizer.getNextToken()) {
            if(t.getType() == TokenType.MULTIPLY) {
                lhs = lhs.multiply(parseIdentifier2(tokenizer.getNextToken()));
            }
            else if(endParen && t.getType() == TokenType.CLOSE_PAREN) {
                break;
            }
            else {
                throw new IllegalArgumentException();
            }
        }
        return lhs;
    }

    public BigInteger evaluateLine2(String line) {
        tokenizer = new Tokenizer(line);
        return parseExpression2(false);
    }
    
    public String partB() {
        BigInteger sum = BigInteger.ZERO;
        for(String line : lines) {
            sum = sum.add(evaluateLine2(line));
        }
        return sum.toString();
     }
     
    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}