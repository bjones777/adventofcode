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
  gate?: string;
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
        if(newTile.gate === gate) {
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
  if(tile.gate === "AA") return sp;
  
  return getGateLocation(maze,tile.gate,sp);
}


function partA() {
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
  lines.forEach((line, y) => {
    for(let x = 0;x < line.length;++x) {
      let s = line[x];
      if(s === ' ') continue;
      if(s === '.') continue; 
      if(s === '#') continue;
      const p = new Point(x,y);
      for(let d = 0;d < 4;++d) {
        const np = moved(p,d);
        const tile = maze.get(np.getKey());
        if(tile != null && tile.type === TileType.open) {
          const od = getOppositeDirection(d);
          const op = moved(p,od);
          const otherLetter = lines[op.y][op.x];
          if(od === Direction.N || od === Direction.W) {
            maze.set(p.getKey(),{ type: TileType.gate, gate: `${otherLetter}${s}`});
          }
          else {
            maze.set(p.getKey(),{ type: TileType.gate, gate: `${s}${otherLetter}`});
          }
        }
      }
    }
  });
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
            line += 'G';
            break;
        }
      }
    }
    console.log(line);
  }

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

rl.on('close', () => {
  try {
    console.log("Part A");
    console.log(partA());
  } catch(e) {
    console.log(e);
  }
});