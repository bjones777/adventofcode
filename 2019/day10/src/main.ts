import * as readline from 'readline';

class Asteroid
{
  public x : number;
  public y : number;

  constructor(x:number,y:number) {
    this.x = x;
    this.y = y;
  }

  getKey() : string {
    return getCoordString(this.x,this.y);
  }
}

function getCoordString(x:number,y:number) : string {
  return `${x.toString()},${y.toString()}`; 
}

function gcd(x : number, y : number) : number {
  x = Math.abs(x);
  y = Math.abs(y);
  while(y) {
    let t = y;
    y = x % y;
    x = t;
  }
  return x;
}

let asteroids = [];

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });


let nextY = 0;
rl.on('line',(line : string) => {
    for(let x = 0;x < line.length;++x) {
      if(line[x] === '#') {
        asteroids.push(new Asteroid(x,nextY));
      }
    } 
    nextY += 1;   
}); 

function calcString(a: Asteroid, b: Asteroid) : string {
  let dx = b.x - a.x;
  let dy = b.y - a.y;
  if(dx != 0 && dy != 0) {
    let d = gcd(dx,dy);
    dx = Math.round(dx/d);
    dy = Math.round(dy/d);
    return getCoordString(dx,dy);
  }
  else if(dx == 0) {
    return getCoordString(0,Math.sign(dy));
  }
  else {
    console.assert(dy == 0);
    return getCoordString(Math.sign(dx),0);
  }
}

let bestAsteroid : Asteroid = null;
let bestSet : Set<string> = null;

function partA() {
  let bestCount = -1;
  for(let i = 0;i < asteroids.length;++i) {
    let set = new Set<string>();
    let a = asteroids[i];
    for(let j = 0;j < asteroids.length;++j) {
      if(i === j) continue;
      let b = asteroids[j];
      set.add(calcString(a,b));
    }
    if(set.size > bestCount) {
      bestAsteroid = a;
      bestCount = set.size;
      bestSet = set;
    }
  }
  console.log(bestCount);
  console.log(`${bestAsteroid.x},${bestAsteroid.y}`);
}

function determineFirstPass(twoSlope,index) : Asteroid{
  let slope = twoSlope[index-1].loc;
  let best200 = null;
  let bestDist = -1;
  for(let j = 0;j < asteroids.length;++j) {
    let b = asteroids[j];
    if(b === bestAsteroid) continue;
      if(calcString(bestAsteroid,b) === slope) {
        if(best200 == null) {
          best200 = b; 
          bestDist = Math.hypot(b.x-bestAsteroid.x,b.y-bestAsteroid.y);
        } else {
          let dist = Math.hypot(b.x-bestAsteroid.x,b.y-bestAsteroid.y);
          if(dist < bestDist) {
            bestDist = dist;
            best200 = b;
          }
        } 
      }
    }
    return best200;
}

function printOridinal(twoSlope,place) 
{
  let a = determineFirstPass(twoSlope,place);
  console.log(`The ${place} asteroid to be vaporized is at ${a.x},${a.y}.`);
}

function partB() {
  // 716 is not right
  let twoSlope = [];
  bestSet.forEach(x => {
    let coords = x.split(',').map(x => parseInt(x,10));
    let angle = Math.atan2(coords[1],coords[0]) + Math.PI/2;
    if(angle < 0) {
      angle += Math.PI * 2;
    }
    twoSlope.push({loc: x, angle: angle});
  });
  twoSlope.sort((a,b) => {
    if(a.angle < b.angle) return -1;
    if(a.anble > b.angle) return 1;
    return 0;
  });

  printOridinal(twoSlope,1);
  printOridinal(twoSlope,2);
  printOridinal(twoSlope,3);
  printOridinal(twoSlope,10);
  printOridinal(twoSlope,20);
  printOridinal(twoSlope,50);
  printOridinal(twoSlope,100);
  printOridinal(twoSlope,199);
  printOridinal(twoSlope,200);
  let a = determineFirstPass(twoSlope,200);
  console.log(`${a.x*100+a.y}`);
}

rl.on('close',() => {
  partA();
  partB();
});