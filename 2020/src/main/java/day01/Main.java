package day01;

import java.io.*;
import java.util.*;
import lombok.val;

public class Main {
    static int partA(List<Integer> numbers) {
        for(int i = 0;i < numbers.size()-1;++i) {
            for(int j = i+1;j < numbers.size();++j)
            if(numbers.get(i).intValue() + numbers.get(j).intValue() == 2020) {
                return numbers.get(i).intValue() * numbers.get(j).intValue();
            }
        }
        return -1;
    }  

    static int partB(List<Integer> numbers) {
        for(int i = 0;i < numbers.size()-2;++i) {
            int ai = numbers.get(i);
            for(int j = i+1; j < numbers.size()-1;++j) {
                int aj = numbers.get(j);
                if(ai + aj >= 2020) {
                    break;
                }
                for(int k = j + 1; k < numbers.size(); ++k) {
                    int ak = numbers.get(k);
                    if(ai + aj + ak == 2020) {
                        return ai*aj*ak;
                    }
                    if(ai + aj + ak > 2020) {
                        break;
                    }
                }
            }
        }
        return -1;
    }

    public static void main(String[] args) throws IOException {
        List<Integer> numbers = new ArrayList<Integer>();
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day01.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                numbers.add(Integer.parseInt(line));
            }
        }
        Collections.sort(numbers);
        System.out.println("Hello from day1");
        System.out.println("partA " + partA(numbers));
        System.out.println("partB " + partB(numbers));
    }
}