import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

class Point {
  constructor(x: number, y:number) {
    this.x = x;
    this.y = y;
  }
  public x : number;
  public y : number;

  static to_string(x : number, y: number) : string {
    return `${x},${y}`;
  }
};

let grid = new Map();
let intersections = new Set();

function mark(dir: string,p: Point, pass : number) : Point {
  let d = dir[0];
  let nx = p.x;
  let ny = p.y;
  let amount = parseInt(dir.substr(1),10);
  while(amount-- > 0) {
    if(d === "U") {
      ny += 1;
      let np = Point.to_string(nx,ny);
      if (grid.has(np) && grid.get(np) !== pass) {
        intersections.add(np);
        grid.set(np, "X");
      } else {
        grid.set(np, pass);
      }
    } else if(d === "D") {
      ny -= 1;
      let np = Point.to_string(nx,ny);
      if (grid.has(np) && grid.get(np) !== pass) {
        intersections.add(np);
        grid.set(np, pass);
      } else {
        grid.set(np, pass);
      }
    } else if(d === "L") {
      nx -= 1;
      let np = Point.to_string(nx,ny);
      if (grid.has(np) && grid.get(np) !== pass) {
        intersections.add(np);
        grid.set(np, pass);
      } else {
        grid.set(np, pass);
      }
    } else if(d === "R") {
      nx += 1;
      let np = Point.to_string(nx,ny);
      if (grid.has(np) && grid.get(np) !== pass) {
        intersections.add(np);
        grid.set(np, pass);
      } else {
        grid.set(np, pass);
      }
    } else {
      throw new Error(`Unknown dir ${d}`);
    }
  }
  return new Point(nx,ny);
}

let grid2 = new Map();
function markB(dir: string,p: Point, pass : number, numSteps: number) : any {
  let d = dir[0];
  let nx = p.x;
  let ny = p.y;
  let amount = parseInt(dir.substr(1),10);
  for(let s = 0; s < amount;++s) {
    if(d === "U") {
      ny += 1;
      let np = Point.to_string(nx,ny);
      if (grid2.has(np)) {
        let ar = grid2.get(np);
        ar[pass] = (ar[pass] == null) ? numSteps + s : Math.min(numSteps+s,ar[pass]);
      } else {
        let ar = [];
        ar[pass] = numSteps + s;
        grid2.set(np, ar);
      }
    } else if(d === "D") {
      ny -= 1;
      let np = Point.to_string(nx,ny);
      if (grid2.has(np)) {
        let ar = grid2.get(np);
        ar[pass] = (ar[pass] == null) ? numSteps + s : Math.min(numSteps+s,ar[pass]);
      } else {
        let ar = [];
        ar[pass] = numSteps + s;
        grid2.set(np, ar);
      }
    } else if(d === "L") {
      nx -= 1;
      let np = Point.to_string(nx,ny);
      if (grid2.has(np)) {
        let ar = grid2.get(np);
        ar[pass] = (ar[pass] == null) ? numSteps + s : Math.min(numSteps+s,ar[pass]);
      } else {
        let ar = [];
        ar[pass] = numSteps + s;
        grid2.set(np, ar);
      }
    } else if(d === "R") {
      nx += 1;
      let np = Point.to_string(nx,ny);
      if (grid2.has(np)) {
        let ar = grid2.get(np);
        ar[pass] = (ar[pass] == null) ? numSteps + s : Math.min(numSteps+s,ar[pass]);
      } else {
        let ar = [];
        ar[pass] = numSteps + s;
        grid2.set(np, ar);
      }
    } else {
      throw new Error(`Unknown dir ${d}`);
    }
  }
  return {np: new Point(nx,ny), steps: numSteps + amount};
}

let pass = 0;
rl.on('line',(line : string) => {
    let p1 = new Point(0,0);
    let p2 = new Point(0,0);
    let dirs = line.split(",");
    let numSteps = 1;
    dirs.forEach((dir) => {
      p1 = mark(dir, p1, pass);
      let o2 = markB(dir,p2,pass,numSteps);
      p2 = o2.np;
      numSteps = o2.steps;
    });
    pass += 1;
}); 

rl.on('close',() => {
  let minDist = null;
  intersections.forEach((p : string) => {
    let dist = p.split(',').map(x => parseInt(x,10)).reduce((x,y)=> x+Math.abs(y),0);
    if(minDist == null || dist < minDist) {
      minDist = dist;
    }
  });
  console.log(minDist);

  let leastSteps = null;
  grid2.forEach((v:number[]) => {
    if(v.length == 2 && v[0] !== undefined) {
      let dist = v.reduce((p,c) => p+c,0);
      if(leastSteps == null || dist < leastSteps) {
        leastSteps = dist;
      }
    }
  });
  console.log(leastSteps);
});