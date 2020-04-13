import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

  let map = new Map<string,string>();
  let keys = new Set<string>();

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

let lineY = 0;
let entrance = new Point();
rl.on('line',(line : string) => {
    for(let x = 0;x < line.length;++x) {
      let key = getCoordKey(x,lineY);
      map.set(key,line[x]);
      if(line[x] !== '@' && line[x] !== '.' && line[x] != '#' && line[x] === line[x].toLowerCase()) {
        keys.add(line[x]);
      }
      if(line[x] === '@') {
        entrance.x = x;
        entrance.y = lineY;
        map.set(key,'.');
      }
    }
    ++lineY;
}); 

function getVisistedString(x : number,y : number,keys: string[]) : string {
  return [x.toString(10), y.toString(10), ...keys].join();
} 

function canMoveTo(loc: Point, keys: string[], visited: Set<string>) : boolean {
  let key = loc.getKey();
  let v = map.get(key);
  if(v === '#') return false;
  if(v === '.' || v === v.toLowerCase() || (v === v.toUpperCase() && keys.includes(v.toLowerCase()))) {
    let vs = getVisistedString(loc.x,loc.y,keys);
    if(!visited.has(vs)) {
      return true;
    }
  }
  return false;
}

function partA() {
  let queue = [];
  let visited = new Set<string>();
  queue.push({x: entrance.x, y: entrance.y, dist:0, keys: []});
  while (queue.length !== 0) {
    let e = queue.shift();
    let vs = getVisistedString(e.x,e.y,e.keys);
    if(visited.has(vs)) continue;
    visited.add(vs);
    let key = getCoordKey(e.x,e.y);
    let value = map.get(key);
    let newKeys = e.keys;
    if(keys.has(value) && !newKeys.includes(value)) {
      newKeys = newKeys.slice();
      newKeys.push(value);
      if(newKeys.length === keys.size) {
        return e.dist;
      }
      newKeys.sort();
    }
    let cp = new Point(e.x,e.y);
    for(let i = 0;i < 4;++i) {
      let np = moved(cp, i);
      if(canMoveTo(np,newKeys,visited)) {
        queue.push({x: np.x, y: np.y, dist: e.dist+1, keys: newKeys});
      }
    }
  }
}

function adjustMap() : Point[] {
  let newPoints = [];
  for(let dy = -1;dy <= 1;++dy) {
    for(let dx = -1;dx <= 1;++dx) {
        if(Math.abs(dy) === 1 && Math.abs(dx) === 1) {
          newPoints.push(new Point(entrance.x+dx,entrance.y+dy));
        }
        else {
          map.set(getCoordKey(entrance.x+dx,entrance.y+dy),'#');
        }
    }
  }
  return newPoints;
}

function getVisistedString2(locs: Point[],keys: string[]) : string {
  return [... locs.map(x=>x.getKey()), ...keys].join();
} 

function canMoveTo2(locs: Point[], index: number, loc: Point, keys: string[], visited: Set<string>) : boolean {
  let key = loc.getKey();
  let v = map.get(key);
  if(v === '#') return false;
  if(v === '.' || v === v.toLowerCase() || (v === v.toUpperCase() && keys.includes(v.toLowerCase()))) {
    let oldP = locs[index];
    locs[index] = loc;
    let vs = getVisistedString2(locs,keys);
    locs[index] = oldP;
    if(!visited.has(vs)) {
      return true;
    }
  }
  return false;
}

function partB() {
  let startPoints = adjustMap();
  let queue = [];
  let visited = new Set<string>();
  queue.push({locs: startPoints, dist:0, keys: []});
  while (queue.length !== 0) {
    let e = queue.shift();
    let vs = getVisistedString2(e.locs, e.keys);
    if(visited.has(vs)) continue;
    visited.add(vs);
    let newKeys = e.keys;
    e.locs.forEach(loc => {
      let value = map.get(loc.getKey());
      if(keys.has(value) && !newKeys.includes(value)) {
        newKeys = newKeys.slice();
        newKeys.push(value);
        newKeys.sort();
      }
    });
    if(newKeys.length === keys.size) {
      return e.dist;
    }
    
    e.locs.forEach((cp,index) => {
      for(let i = 0;i < 4;++i) {
        let np = moved(cp, i);
        if(canMoveTo2(e.locs,index,np,newKeys,visited)) {
          let newLocs = e.locs.slice();
          newLocs[index] = np;
          queue.push({locs: newLocs, dist: e.dist+1, keys: newKeys});
        }
      }
    });
  }
}

rl.on('close',() => {
  console.log(partA());
  console.log(partB());
});