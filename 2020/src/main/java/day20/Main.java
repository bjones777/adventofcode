package day20;

import java.util.*;
import java.util.regex.*;
import java.util.stream.Collectors;
import java.io.*;
import java.math.BigInteger;

import lombok.*;


public class Main {
    private static final int DIM = 10;
    private static final int IMAGE_DIM = 12;
    private static final int FINAL_DIM = IMAGE_DIM * (DIM-2);
    
    @Value 
    static class Layout {
        int config;
        Tile piece;
    }

    @Value
    static class Edge {
        String pattern;

        Edge(String inPattern) {
            val sb = new StringBuilder();
            for(int i = inPattern.length() - 1;i >= 0;--i) {
                sb.append(inPattern.charAt(i));
            }
            String rev = sb.toString();
            pattern = (rev.compareTo(inPattern) < 0) ? rev : inPattern;
        }
    }

    static class Grid {
        Layout[][] tiles = new Layout[IMAGE_DIM][IMAGE_DIM];
        Set<Tile> edgePieces;

        List<Tile> pieces;
        List<Boolean> used;
        
        Tile getCorner(int corner) {
            int row = corner / 2;
            int col = corner % 2;

            return tiles[row * (IMAGE_DIM-1)][col * (IMAGE_DIM-1)].getPiece();
        } 

        boolean fill(List<Tile> inPieces) {
            used = new ArrayList<>();
            for(int i = 0;i < inPieces.size();++i) {
                used.add(false);
            }
            pieces = inPieces;
            identifyUniqueEdges();
            return fill(0,0);
        }

        void identifyUniqueEdges() {
            edgePieces = new HashSet<>();
            for(int i = 0;i < pieces.size();++i) {
                Set<Edge> uniqueEdges = pieces.get(i).getUniqueEdges();
                for(int j = 0;j < pieces.size();++j) {
                    if(i == j) continue;
                    uniqueEdges.removeAll(pieces.get(j).getUniqueEdges());
                    if(uniqueEdges.isEmpty()) {
                        break;
                    }
                }
                if(!uniqueEdges.isEmpty()) {
                    edgePieces.add(pieces.get(i));
                }
            }
        }

        boolean fill(int row, int col) {
            if(col >= IMAGE_DIM) {
                ++row;
                col = 0;
            }
            if(row >= IMAGE_DIM) {
                return true;
            } 
            for(int i = 0;i < pieces.size();++i) {
                if(used.get(i)) continue;
                Tile t = pieces.get(i);

                for(int c = 0;c < t.getNumConfigs();++c) {
                    if(canFill(t,row,col,c)) {
                        used.set(i,true);
                        tiles[row][col] = new Layout(c,t);
                        if(fill(row,col+1)) {
                            return true;
                        }
                        tiles[row][col] = null;
                        used.set(i,false);
                    }
                }   
            }
            return false;
        }

        boolean canFill(Tile t,int row, int col, int config) {
            boolean isEdge = (col == 0 || col == IMAGE_DIM-1) || (row == 0 || row == IMAGE_DIM-1);
            boolean canEdge = edgePieces.contains(t);
            if(isEdge != canEdge) {
                return false;
            }
            if(col != 0) {
                val layout = tiles[row][col-1];
                if(!doesMatchLeftRight(layout.getPiece(), layout.getConfig(), t, config)) {
                    return false;
                }
            }
            if(row != 0) {
                val layout = tiles[row-1][col];
                if(!doesMatchTopBottom(layout.getPiece(), layout.getConfig(), t, config)) {
                    return false;
                }
            }
            return true;
        }

        boolean doesMatchLeftRight(Tile left, int lc, Tile right, int rc) {
            return left.matchesLeftRight(lc, right, rc);
        }

        boolean doesMatchTopBottom(Tile left, int lc, Tile right, int rc) {
            return left.matchesTopBottom(lc, right, rc);
        }

        Image buildImage() {
            char[][] image = new char[FINAL_DIM][FINAL_DIM];
            for(int i = 0;i < IMAGE_DIM;++i) {
                for(int j = 0;j < IMAGE_DIM;++j) {
                    val layout = tiles[i][j];
                    char[][] config = layout.getPiece().configs.get(layout.getConfig()); 
                    for(int k = 1;k < DIM-1;++k) {
                        for(int l = 1;l < DIM-1;++l) {
                            char dest = image[i*(DIM-2)+(k-1)][j*(DIM-2)+(l-1)];
                            if(((int)dest) != 0) {
                                throw new RuntimeException();
                            } 
                            image[i*(DIM-2)+(k-1)][j*(DIM-2)+(l-1)] = config[k][l];
                        }
                    }
                }
            }
            for(int i = 0;i < FINAL_DIM;++i) {
                for(int j = 0;j < FINAL_DIM;++j) {
                    if(((int)image[i][j]) == 0) {
                        throw new RuntimeException();
                    }
                }
            }
            return new Image(image);
        }
    }

    static class Tile {
        int id;
        List<char[][]> configs = new ArrayList<>();
        List<Edge> edges;

        int getNumConfigs() {
            return configs.size();
        }

        public List<Edge> getEdges() {
            return edges;
        }

        public Set<Edge> getUniqueEdges() {
            return edges.stream().collect(Collectors.toSet());
        }

        BigInteger getIdAsBigInt() {
            return new BigInteger(Integer.toString(id));
        }

        public Tile(int inId, char[][] inLines) {
            id = inId;
            configs.add(inLines);
            for(int i = 0;i < 3;++i) {
                configs.add(rotateRight(configs.get(configs.size()-1)));
            }
            for(int i = 0;i < 4;++i) {
                configs.add(flipHorizontal(configs.get(i)));
            }
            for(int i = 0;i < 4;++i) {
                configs.add(flipVertical(configs.get(i)));
            }
            buildEdges();
        }   

        private void buildEdges() {
            char[][] config = configs.get(0); 
            edges = new ArrayList<>();
            // top
            {
                val sb = new StringBuilder();
                for(int i = 0;i < DIM;++i) {
                    sb.append(config[0][i]);
                }
                edges.add(new Edge(sb.toString()));
            }
            // bottom
            {
                val sb = new StringBuilder();
                for(int i = 0;i < DIM;++i) {
                    sb.append(config[DIM-1][i]);
                }
                edges.add(new Edge(sb.toString()));
            }
            // left
            {
                val sb = new StringBuilder();
                for(int i = 0;i < DIM;++i) {
                    sb.append(config[i][0]);
                }
                edges.add(new Edge(sb.toString()));
            }
            // right
            {
                val sb = new StringBuilder();
                for(int i = 0;i < DIM;++i) {
                    sb.append(config[i][DIM-1]);
                }
                edges.add(new Edge(sb.toString()));
            }
        }

        public boolean matchesLeftRight(int config,Tile other, int otherConfigIndex) {
            char[][] myConfig = configs.get(config);
            char[][] otherConfig = other.configs.get(otherConfigIndex);
            for(int i = 0;i < DIM;++i) {
                if(myConfig[i][DIM-1] != otherConfig[i][0]) {
                    return false;
                }
            }
            return true;
        }

        public boolean matchesTopBottom(int config,Tile other, int otherConfigIndex) {
            char[][] myConfig = configs.get(config);
            char[][] otherConfig = other.configs.get(otherConfigIndex);
            for(int j = 0;j < DIM;++j) {
                if(myConfig[DIM-1][j] != otherConfig[0][j]) {
                    return false;
                }
            }
            return true;
        }

        private static char[][] rotateRight(char[][] lines) {
            char[][] rotated = new char[DIM][DIM];
            for(int i = 0;i < DIM;++i) {
                for(int j = 0;j < DIM;++j) {
                    rotated[i][j] = lines[DIM-1-j][i];
                }
            }
            return rotated;
        }

        private static char[][] flipVertical(char[][] lines) {
            char[][] rotated = new char[DIM][DIM];
            for(int i = 0;i < DIM;++i) {
                for(int j = 0;j < DIM;++j) {
                    rotated[i][j] = lines[DIM-1-i][j];
                }
            }
            return rotated;
        }

        private static char[][] flipHorizontal(char[][] lines) {
            char[][] rotated = new char[DIM][DIM];
            for(int i = 0;i < DIM;++i) {
                for(int j = 0;j < DIM;++j) {
                    rotated[i][j] = lines[i][DIM-1-j];
                }
            }
            return rotated;
        }
    }

    // Tile 3079:
    private static final Pattern tilePattern = Pattern.compile("Tile ([0-9]+):");
    private static List<List<Tile>> picture = new ArrayList<>();
    List<Tile> tiles = new ArrayList<>();

    public Main() {   
        for(int i = 0;i < IMAGE_DIM;++i) {
            picture.add(new ArrayList<>());
            val lastArray = picture.get(picture.size()-1);
            for(int j = 0;j < IMAGE_DIM;++j) {
                lastArray.add(null);
            }
        }
    }

    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day20.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                val m = tilePattern.matcher(line);
                if(!m.matches()) {
                    throw new RuntimeException("Unknown line: " + line);
                }
                int id = Integer.parseInt(m.group(1));
                char[][] chars = new char[DIM][DIM];
                for(int i = 0;i < DIM;++i) {
                    line = br.readLine();
                    for(int j = 0;j < DIM;++j) {
                        chars[i][j] = line.charAt(j);
                    }
                }

                tiles.add(new Tile(id,chars));
                line = br.readLine();
                if(line == null) {
                    break;
                }
                if(!line.isBlank()) {
                    throw new RuntimeException("Unexpected line: " + line);
                }
            }
       }
    }



    public String partA() {
        Grid g = new Grid();
        if(g.fill(tiles)) {
            BigInteger b = BigInteger.ONE;
            for(int i = 0;i < 4;++i) {
                b = b.multiply(g.getCorner(i).getIdAsBigInt());
            }
            return b.toString();
        }
        return "Couldn't fill";
    }

    public static class Image {
        List<char[][]> configs = new ArrayList<>();

        public int getNumConfigs() {
            return configs.size();
        }

        public Image(char[][] inImage) {
            
            configs.add(inImage);
            for(int i = 0;i < 3;++i) {
                configs.add(rotateRight(configs.get(configs.size()-1)));
            }
            for(int i = 0;i < 4;++i) {
                configs.add(flipHorizontal(configs.get(i)));
            }
            for(int i = 0;i < 4;++i) {
                configs.add(flipVertical(configs.get(i)));
            }
        }   

        char get(int row, int col,int config)  {
            char[][] image = configs.get(config); 
            if(row >= FINAL_DIM) {
                return '-';
            }
            if(col >= FINAL_DIM) {
                return '-';
            }
            return image[row][col];
        }

        void set(int row, int col, int config,char ch)  {
            char[][] image = configs.get(config); 
            image[row][col] = ch;
        }

        private static char[][] rotateRight(char[][] lines) {
            char[][] rotated = new char[FINAL_DIM][FINAL_DIM];
            for(int i = 0;i < FINAL_DIM;++i) {
                for(int j = 0;j < FINAL_DIM;++j) {
                    rotated[i][j] = lines[FINAL_DIM-1-j][i];
                }
            }
            return rotated;
        }

        private static char[][] flipVertical(char[][] lines) {
            char[][] rotated = new char[FINAL_DIM][FINAL_DIM];
            for(int i = 0;i < FINAL_DIM;++i) {
                for(int j = 0;j < FINAL_DIM;++j) {
                    rotated[i][j] = lines[FINAL_DIM-1-i][j];
                }
            }
            return rotated;
        }

        private static char[][] flipHorizontal(char[][] lines) {
            char[][] rotated = new char[FINAL_DIM][FINAL_DIM];
            for(int i = 0;i < FINAL_DIM;++i) {
                for(int j = 0;j < FINAL_DIM;++j) {
                    rotated[i][j] = lines[i][FINAL_DIM-1-j];
                }
            }
            return rotated;
        }

        public void printConfig(int config) {
            char[][] image = configs.get(0); 
            for(int i = 0;i < FINAL_DIM;++i) {
                for(int j = 0;j < FINAL_DIM;++j) {
                    System.out.print(image[i][j]);
                }
                System.out.println();
            }
        }
    }

    public static class Monster {
        List<String> points = new ArrayList<>();
        final int numRows;        
        final int numCols;

        public Monster() {
            //points.add("                  # ");
            //points.add("#    ##    ##    ###");
            //points.add(" # #  #");
             points.add("                  # ");
             points.add("#    ##    ##    ###");
             points.add(" #  #  #  #  #  #   ");  
             numRows = points.size();
             numCols = points.get(0).length();

        }

        public boolean matches(Image image,int row,int col,int config) {
            for(int dr = 0;dr < numRows;++dr) {
                String line = points.get(dr);
                for(int dc = 0;dc < numCols;++dc) {
                    char ch = line.charAt(dc);
                    char ich = image.get(row+dr,col+dc, config);
                    if(ich == '-') {
                        return false;
                    }
                    if(ch == '#' && ich != '#') {
                        return false;
                    }
                }
            }
            return true;
        }

        public void drawMonster(Image image, MonsterLoc monsterLoc) {
            int config = monsterLoc.getConfig();
            int row = monsterLoc.getRow();
            int col = monsterLoc.getCol();
            for(int dr = 0;dr < numRows;++dr) {
                String line = points.get(dr);
                for(int dc = 0;dc < numCols;++dc) {
                    char ch = line.charAt(dc);
                    if(ch != '#') {
                        continue;
                    }
                    char ich = image.get(row+dr,col+dc, config);
                    if(ich != '#') {
                        throw new RuntimeException();
                    }
                    image.set(row+dr, col+dc, config, 'O');
                }
            }
        }
    }

    @Value
    public static class MonsterLoc {
        int row;
        int col;
        int config;
    }


    public List<MonsterLoc> findAndDrawMonster(Image image,Monster m) {
        List<MonsterLoc> monsterLocs = new ArrayList<>();
        for(int con = 0;con < image.getNumConfigs();++con) {
            //if(con != 0) {
            //    System.out.println();
            //}
            //image.printConfig(con);
            for(int i = 0;i < FINAL_DIM;++i) {
                for(int j = 0;j < FINAL_DIM;++j) {
                    if(m.matches(image, i, j, con)) {
                        val ml = new MonsterLoc(i,j,con);
                        m.drawMonster(image, ml);
                        monsterLocs.add(ml);
                    }
                }
            }
        }
        return monsterLocs;
    }

    public String partB() {
        Grid g = new Grid();
        if(g.fill(tiles)) {
            val image = g.buildImage();
            Monster m = new Monster();
            val monsterLocs = findAndDrawMonster(image, m);

            int numHash = 0;
            int config = monsterLocs.get(0).getConfig();
            for(int i = 0;i < FINAL_DIM;++i) {
                for(int j = 0;j < FINAL_DIM;++j) {
                    if(image.get(i, j, config) == '#') {
                        ++numHash;
                    }
                }
            }
            return Integer.toString(numHash);
        }
        return "Couldn't fill";
     }

     
     
    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}