import * as readline from 'readline';

class Point {
  public x : number;
  public y : number;

  constructor(x:number = 0, y:number = 0) {
    this.x = x;
    this.y = y;
  }

  getKey() : string {
      return `${this.x.toString()},${this.y.toString()}`; 
  }
}

function getCoordKey(x : number, y: number) : string {
  return `${x.toString()},${y.toString()}`; 
}

enum Direction {
  N, S, W, E
};

function getOppositeDirection(d : Direction) : Direction {
  switch(d) {
    case Direction.N:
      return Direction.S;
    case Direction.S:
      return Direction.N;
    case Direction.E:
      return Direction.W;
    case Direction.W:
      return  Direction.E;
  }
}


function move(p : Point,d : Direction) {
  if(d === Direction.N) {
    p.y -= 1;
  } else if(d === Direction.S) {
    p.y += 1;
  } else if(d === Direction.E) {
    p.x += 1;
  } else {
    if(d !== Direction.W) {
      console.assert(false);
    }
    p.x -= 1;
  }
}

function moved(p : Point, d : Direction) : Point {
  let np = new Point(p.x,p.y);
  move(np, d);
  return np; 
}


let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

enum TileType {
  open,
  closed,
  gate,
}

interface Tile {
  type : TileType;
  gate?: { name: string, depthChange: number};
}

const lines : string[] = [];

rl.on('line',(line : string) => {
    lines.push(line);  
}); 

function getGateLocation(maze : Map<string,Tile>, gate: string, ignorePoint?: Point) : Point {
  for(const key of Array.from(maze.keys())) {
    const tile = maze.get(key);
    if(tile.type === TileType.open) {
      let coord = key.split(',');
      const p = new Point(Number(coord[0]),Number(coord[1]));
      if(ignorePoint != null && p.x === ignorePoint.x && p.y === ignorePoint.y) {
        continue;
      }
      for(let d = 0;d < 4;++d) {
        const np = moved(p,d);
        let newTile = maze.get(np.getKey());
        if(newTile == null) continue;
        if(newTile.gate == null) continue;
        if(newTile.gate.name === gate) {
          return p;
        }
      }
    }
  }
  throw new Error("Could not find staring point");
}

function getStartLocation(maze : Map<string,Tile>) : Point {
  return getGateLocation(maze, "AA");
}

function getEndLocation(maze : Map<string,Tile>) : Point {
  return getGateLocation(maze, "ZZ");
}

function moveInMaze(maze : Map<string,Tile>,sp : Point,d : Direction) : Point {
  const np = moved(sp,d);
  let tile = maze.get(np.getKey());
  if(tile.type === TileType.closed) return sp;
  if(tile.type === TileType.open) return np;
  if(tile.gate == null) throw new Error("What is this?");
  if(tile.gate.name === "AA") return sp;
  
  return getGateLocation(maze,tile.gate.name,sp);
}

function createMaze() : Map<string,Tile> {
  const maze = new Map<string,Tile>();
  lines.forEach((line, y) => {
    for(let x = 0;x < line.length;++x) {
      let s = line[x];
      if(s === ' ') continue;
      if(s === '.') {
        let key = getCoordKey(x,y);
        maze.set(key,{type: TileType.open});
      }
      else if(s === '#') {
        let key = getCoordKey(x,y);
        maze.set(key,{type: TileType.closed});
      }
    }
  });
  let NW = new Point(-1,-1);
  let SW = new Point(-1,-1);
  let NE = new Point(-1,-1);
  let SE = new Point(-1,-1);
  maze.forEach((value: Tile, key: string) => {
    let coord = key.split(',');
    const p = new Point(Number(coord[0]),Number(coord[1]));
    const notHasNorth = !maze.has(moved(p, Direction.N).getKey()); 
    const notHasSouth = !maze.has(moved(p, Direction.S).getKey()); 
    const notHasEast = !maze.has(moved(p, Direction.E).getKey());
    const notHasWest = !maze.has(moved(p, Direction.W).getKey());
    if(notHasNorth && notHasEast) {
      NE = p;
    }
    if(notHasNorth && notHasWest) {
      NW = p;
    }
    if(notHasSouth && notHasEast) {
      SE = p;
    }
    if(notHasSouth && notHasWest) {
      SW = p;
    }
  });
  const minX = NW.x;
  const maxX = NE.x;
  const minY = NW.y;
  const maxY = SW.y;
  
  lines.forEach((line, y) => {
    for(let x = 0;x < line.length;++x) {
      let s = line[x];
      if(s === ' ') continue;
      if(s === '.') continue; 
      if(s === '#') continue;
      const p = new Point(x,y);
      let depthChange = 1;
      if(p.x < minX || p.x > maxX || p.y < minY || p.y > maxY) {
        depthChange = -1;
      }
      for(let d = 0;d < 4;++d) {
        const np = moved(p,d);
        const tile = maze.get(np.getKey());
        if(tile != null && tile.type === TileType.open) {
          const od = getOppositeDirection(d);
          const op = moved(p,od);
          const otherLetter = lines[op.y][op.x];
          if(od === Direction.N || od === Direction.W) {
            maze.set(p.getKey(),{ type: TileType.gate, gate: {name: `${otherLetter}${s}`, depthChange: depthChange}});
          }
          else {
            maze.set(p.getKey(),{ type: TileType.gate, gate: {name: `${s}${otherLetter}`, depthChange: depthChange}});
          }
        }
      }
    }
  });
  return maze;
}

function printMaze(maze : Map<string,Tile>) {
  for(let y = 0;y < 22;++y) {
    let line = '';
    for(let x = 0;x < 22;++x) {
      const tile = maze.get(getCoordKey(x,y));
      if(tile == null) {
        line += ' ';
      }
      else {
        switch(tile.type) {
          case TileType.closed:
            line += '#';
            break;
          case TileType.open:
            line += '.';
            break;
          case TileType.gate:
            if(tile.gate == null) {
              throw new Error("gate is not defined")
            }
            if(tile.gate.depthChange < 1) {
              line += 'G';
            }
            else {
              line += 'g';
            }
            break;
        }
      }
    }
    console.log(line);
  }
}

function moveInMazeB(maze : Map<string,Tile>, sp : Point,depth: number,d : Direction) : { point: Point, depth: number } {
  const np = moved(sp,d);
  let tile = maze.get(np.getKey());
  if(tile.type === TileType.closed) return { point: sp, depth: depth};
  if(tile.type === TileType.open) return { point: np, depth: depth };
  if(tile.gate == null) throw new Error("What is this?");
  if(tile.gate.name === "AA") return { point: sp, depth: depth};
  if(tile.gate.name === "ZZ") return { point: sp, depth: depth};

  const newDepth = depth + tile.gate.depthChange;
  if(newDepth < 0) {
    return { point: sp, depth: depth};
  }
  
  return { point: getGateLocation(maze,tile.gate.name,sp), depth: newDepth };
}

function partA() {
  const maze = createMaze(); 
  printMaze(maze);

  const endLoc = getEndLocation(maze);
  const visited = new Set<string>();
  const toVisit = [{loc: getStartLocation(maze), numMoves: 0 }];
  while(toVisit.length > 0) {
    const curr = toVisit.shift();
    console.log(`loc: ${curr.loc.getKey()}, numMoves: ${curr.numMoves}`);
    if(curr.loc.x == endLoc.x && curr.loc.y === endLoc.y) {
      return curr.numMoves;
    }
    if(visited.has(curr.loc.getKey())) {
      continue;
    }
    visited.add(curr.loc.getKey());
    for(let d = 0;d < 4;++d) {
      const np = moveInMaze(maze,curr.loc,d);
      if(visited.has(np.getKey())) {
        continue;
      }
      toVisit.push({loc: np, numMoves: curr.numMoves+1});
    }
  }
}

function getVisitedKey(p : Point, d : number) : string {
  return `${p.getKey()},${d}`;
}

function partB() {
  const maze = createMaze(); 
  const endLoc = getEndLocation(maze);
  const visited = new Set<string>();
  const toVisit = [{loc: getStartLocation(maze), depth: 0, numMoves: 0 }];
  while(toVisit.length > 0) {
    const curr = toVisit.shift();
    //console.log(`loc: ${curr.loc.getKey()}, depth: ${curr.depth}, numMoves: ${curr.numMoves}`);
    if(curr.loc.x == endLoc.x && curr.loc.y === endLoc.y && curr.depth === 0) {
      return curr.numMoves;
    }
    const visitedKey = getVisitedKey(curr.loc,curr.depth);
    if(visited.has(visitedKey)) {
      continue;
    }
    visited.add(visitedKey);
    for(let d = 0;d < 4;++d) {
      const np = moveInMazeB(maze,curr.loc,curr.depth,d);
      if(visited.has(getVisitedKey(np.point,np.depth))) {
        continue;
      }
      toVisit.push({loc: np.point, depth: np.depth, numMoves: curr.numMoves+1});
    }
  }
}

rl.on('close', () => {
  try {
    console.log("Part A");
    console.log(partA());
    console.log("Part B");
    console.log(partB());
  } catch(e) {
    console.log(e);
  }
});