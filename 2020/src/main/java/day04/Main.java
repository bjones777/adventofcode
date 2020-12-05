package day04;

import java.util.*;
import java.util.regex.*;
import java.io.*;
import lombok.*;

public class Main {
    public static class Passport {
        static Set<String> requiredFields = new HashSet<String>(Arrays.asList("byr"
        ,"iyr"
        ,"eyr"
        ,"hgt"
        ,"hcl"
        ,"ecl"
        ,"pid"));

        static Set<String> eyeColorSet = new HashSet<String>(Arrays.asList(
            "amb", "blu", "brn", "gry", "grn", "hzl", "oth"
        ));
        Pattern colorPattern = Pattern.compile("#[0-9a-f]{6}");
        Pattern passportIdPattern = Pattern.compile("[0-9]{9}");
        Pattern heightPattern = Pattern.compile("([0-9]+)(in|cm)");
        Map<String, String> keyValues = new HashMap<>();
        
        public void addPair(String key, String value) {
            keyValues.put(key,value);
        }

        public boolean isValid() {
            return keyValues.keySet().containsAll(requiredFields);
        }

        private boolean validateBirthYear() {
            String v = keyValues.get("byr");
            if(v.length() != 4) return false;
            try {
                int val = Integer.parseInt(v);
                return val >= 1920 && val <= 2002;
            }
            catch(Exception e) {}
            return false;

        }

        public boolean validateIssueYear() {
            String v = keyValues.get("iyr");
            if(v.length() != 4) return false;
            try {
                int val = Integer.parseInt(v);
                return val >= 2010 && val <= 2020;
            }
            catch(Exception e) {}
            return false;

        }

        public boolean validateExpirationYear() {
            String v = keyValues.get("eyr");
            if(v.length() != 4) return false;
            try {
                int val = Integer.parseInt(v);
                return val >= 2020 && val <= 2030;
            }
            catch(Exception e) {}
            return false;

        }

        public boolean validateHeight() {
            String hgt = keyValues.get("hgt");

            val m = heightPattern.matcher(hgt);
            if(m.matches()) {
                if(m.group(2).equals("cm")) {
                    int v = Integer.parseInt(m.group(1));
                    return v >= 150 && v <= 193;
                }
                if(m.group(2).equals("in")) {
                    int v = Integer.parseInt(m.group(1));
                    return v >= 59 && v <= 76;
                }
                throw new RuntimeException("How do we handle" + hgt);
            }
            return false;        
        }

        public boolean validateHairColor() {
            String hcl = keyValues.get("hcl");
            
            val m = colorPattern.matcher(hcl);
            return m.matches();
        }

        public boolean validateEyeColor() {
            String ecl = keyValues.get("ecl");
            
            return eyeColorSet.contains(ecl);
        }

        public boolean validatePassportId() {
            String pid = keyValues.get("pid");
            
            val m = passportIdPattern.matcher(pid);
            return m.matches();

        }

        public boolean isValid2() {
            if(!isValid()) return false;

            if(!validateBirthYear()) return false;
            if(!validateIssueYear()) return false;
            if(!validateExpirationYear()) return false;
            if(!validateHeight()) return false;
            if(!validateHairColor()) return false;
            if(!validateEyeColor()) return false;
            if(!validatePassportId()) return false;
            
            return true;
        }
    }

    public static void main(String[] args) throws IOException {
        System.out.println("Hello from day4");
        List<Passport> passports = new ArrayList<>();
        Passport buildingPassport = null;
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day04.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                String[] pairs = line.split("\\s");
                int numPairs = 0;
                for(String pair : pairs) {
                    String[] kvp = pair.split(":");
                    if(kvp.length == 2) {
                        if(buildingPassport == null) {
                            buildingPassport = new Passport();
                        }
                        ++numPairs;
                        buildingPassport.addPair(kvp[0], kvp[1]);
                    }
                }    
                if(numPairs == 0 && buildingPassport != null)  {
                    passports.add(buildingPassport);
                    buildingPassport = null;
                }
            }
        }
        if(buildingPassport != null) {
            passports.add(buildingPassport);
            buildingPassport = null;
        }

        System.out.println(String.format("part A: %d", passports.stream().filter(p -> p.isValid()).count()));
        System.out.println(String.format("part B: %d", passports.stream().filter(p -> p.isValid2()).count()));
    }
}