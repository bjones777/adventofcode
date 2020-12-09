package day08;

import java.util.*;
import java.io.*;

import lombok.*;

public class Main {
    public static enum InstructionType {
        NOP,
        ACC,
        JMP 
    }

    public static InstructionType toInstructionType(String v) {
        if(v.equals("acc")) {
            return InstructionType.ACC;
        }
        if(v.equals("nop")) {
            return InstructionType.NOP;
        }
        if(v.equals("jmp")) {
            return InstructionType.JMP;
        }
        throw new RuntimeException("Unknown value " + v);
    }

    @Builder
    @Value
    public static class Instruction {
          InstructionType type;
          int immediate;      
    }

    @Builder
    @Value
    public static class Pair {
        int ip;
        int accum;
    } 

    public static class Simulator {
        private int ip = 0;
        private List<Instruction> instructions;
        private int accum = 0;

        public Simulator(List<Instruction> in) {
            instructions = in;
        }

        public int partA() {
            Set<Integer> pairs = new HashSet<Integer>();
            for(;;) {
                doStep();
                if(pairs.contains(ip)) {
                    return accum;
                }
                pairs.add(ip);
            }
        }

        private void doStep() {
            val instr = instructions.get(ip);
            switch(instr.getType()) {
                case ACC:
                    accum += instr.getImmediate();
                    ip += 1;
                    break;
                case NOP:
                    ip += 1;
                    break;
                case JMP:
                    ip += instr.getImmediate();
                    break;
            }
        }

        public int partB() {
            for(int i = 0;i < instructions.size();++i) {
                val instr = instructions.get(i);
                if(instr.getType() == InstructionType.NOP) {
                    val newInst = Instruction.builder().type(InstructionType.JMP).immediate(instr.getImmediate()).build();
                    instructions.set(i,newInst);
                    if(tryRun()) {
                        return accum;
                    }
                    instructions.set(i,instr);
                }
                else if(instr.getType() == InstructionType.JMP) {
                    val newInst = Instruction.builder().type(InstructionType.NOP).immediate(instr.getImmediate()).build();
                    instructions.set(i,newInst);
                    if(tryRun()) {
                        return accum;
                    }
                    instructions.set(i,instr);
                }
            }
            throw new RuntimeException("Didn't find termination");
        }

        public boolean tryRun() {
            ip = 0;
            accum = 0;
            final int MAX_INSTR = 100000;
            for(int i = 0;i < MAX_INSTR;++i) {
                if(ip >= 0 && ip < instructions.size()) {
                    doStep();
                }
                else {
                    return true;
                }
            }
            return false;
        }
    }

    
    public static void main(String[] args) throws IOException {
        List<Instruction> instructions = new ArrayList<>();
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day08.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                String[] split = line.split(" ");
                if(split.length != 2) {
                    throw new RuntimeException("Unknown line" + line);
                }
                instructions.add(Instruction.builder().type(toInstructionType(split[0])).immediate(Integer.parseInt(split[1])).build());
            }
        }
        Simulator s = new Simulator(instructions);
        System.out.println(String.format("Part A: %d", s.partA()));
        System.out.println(String.format("Part B: %d", s.partB()));

    }
}