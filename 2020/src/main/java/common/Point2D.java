package common;

import lombok.*;

@Value
public class Point2D {
    int x;
    int y;

    public int getManhattanDistance() {
        return Math.abs(x) + Math.abs(y);
    }

    public Point2D moved(Direction d, int amount) {
        switch(d) {
            case N:
                return new Point2D(x, y - amount);
            case S:
                return new Point2D(x, y + amount);
            case E:
                return new Point2D(x + amount, y);
            case W:
                return new Point2D(x - amount, y);
        }
        throw new RuntimeException("Bad direction");
    }

    public Point2D add(Point2D p) {
        return new Point2D(x + p.x, y+p.y);
    }
}
