package common;

public enum Direction {
        N,
        S,
        E,
        W;

        public Direction turnedRight() {
            return turnRight(this);
        }

        public Direction turnedLeft() {
            return turnLeft(this);
        }

        public static Direction turnLeft(Direction d) {
            switch(d) {
                case N:
                    return Direction.W;
                case S:
                    return Direction.E;
                case E:
                    return Direction.N;
                case W:
                    return Direction.S;
            }
            throw new RuntimeException("Bad direction");
        }
    
        public static Direction turnRight(Direction d) {
            switch(d) {
                case N:
                    return Direction.E;
                case S:
                    return Direction.W;
                case E:
                    return Direction.S;
                case W:
                    return Direction.N;
            }
            throw new RuntimeException("Bad direction");
        }
}
