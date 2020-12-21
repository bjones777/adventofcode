package day21;

import java.util.*;
import java.util.regex.*;
import java.util.stream.Collectors;
import java.io.*;

import lombok.*;


public class Main {

    public static Pattern linePattern = Pattern.compile("(.*) \\(contains (.*)\\)");

    public static class Food {
        Set<String> ingredients;
        Set<String> allergens;

        public Food(String line) {
            val matcher = linePattern.matcher(line);
            if(!matcher.matches()) {
                throw new RuntimeException();
            }
            ingredients = Arrays.stream(matcher.group(1).split(" ")).collect(Collectors.toSet());
            allergens = Arrays.stream(matcher.group(2).split(", ")).collect(Collectors.toSet());
        }

        public boolean isConsistent(Map<String,String> allergenMap) {
            return allergenMap.entrySet().stream().allMatch(kvp -> 
                !allergens.contains(kvp.getKey()) || 
                (allergens.contains(kvp.getKey()) && ingredients.contains(kvp.getValue())));
        }
    }

    
    List<Food> foods = new ArrayList<>();
    public Main() {   
    }

    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day21.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                foods.add(new Food(line));
            }
       }
    }

    public String partA() {
        Set<String> allAllergens = foods.stream().flatMap(f -> f.allergens.stream()).collect(Collectors.toSet());
        Set<String> allIngredients = foods.stream().flatMap(f -> f.ingredients.stream()).collect(Collectors.toSet());

        Set<String> cantAppear = new HashSet<>();
        for(String ingredient : allIngredients) {
            boolean doAny = allAllergens.stream().anyMatch(allergen -> {
                Map<String,String> allergenMap = new HashMap<>();
                allergenMap.put(allergen, ingredient);
                return foods.stream().allMatch(f -> f.isConsistent(allergenMap));
            });
            if(!doAny) {
                cantAppear.add(ingredient);
            }
        }
        int cnt = 0;
        for(val food : foods) {
            for(String i : food.ingredients) {
                if(cantAppear.contains(i)) {
                    ++cnt;
                }
            }
        } 
        return Integer.toString(cnt);
    }

    
    public String partB() {
        Set<String> allAllergens = foods.stream().flatMap(f -> f.allergens.stream()).collect(Collectors.toSet());
        Set<String> allIngredients = foods.stream().flatMap(f -> f.ingredients.stream()).collect(Collectors.toSet());

        Set<String> cantAppear = new HashSet<>();
        for(String ingredient : allIngredients) {
            boolean doAny = allAllergens.stream().anyMatch(allergen -> {
                Map<String,String> allergenMap = new HashMap<>();
                allergenMap.put(allergen, ingredient);
                return foods.stream().allMatch(f -> f.isConsistent(allergenMap));
            });
            if(!doAny) {
                cantAppear.add(ingredient);
            }
        }
        Set<String> badIngredients = new HashSet<>();
        badIngredients.addAll(allIngredients);
        badIngredients.removeAll(cantAppear);

        Map<String,String> allergenMap = new HashMap<>();
        if(assign(badIngredients,allAllergens,allergenMap)) {
            String[] keys = new String[allergenMap.size()];
            allergenMap.keySet().toArray(keys);
            Arrays.sort(keys);
            return Arrays.stream(keys).map(k -> allergenMap.get(k)).collect(Collectors.joining(","));
        }
        return "";
        
     }

     public boolean assign(Set<String> ingredients, Set<String> allergens, Map<String,String> allergenMap) {
        if(allergenMap.size() == ingredients.size()) {
            return foods.stream().allMatch(f -> f.isConsistent(allergenMap));
        }
        for(String allergen : allergens) {
            if(allergenMap.containsKey(allergen)) continue;

            for(String ingredient : ingredients) {
                if(allergenMap.containsValue(ingredient)) continue;

                allergenMap.put(allergen, ingredient);
                if(foods.stream().allMatch(f -> f.isConsistent(allergenMap))) {
                    if(assign(ingredients, allergens, allergenMap)) {
                        return true;
                    }
                }
                allergenMap.remove(allergen);
            }
        }
        return false;
     }

     
     
    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}