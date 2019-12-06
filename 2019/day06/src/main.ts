import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

let map = new Map<string,Set<string>>();
let rmap = new Map<string,Set<string>>();
let names = new Set<string>();

function addTo(m: Map<string,Set<string>>,s : string, d : string) {
  if (m.has(s)) {
    let set = m.get(s);
    set.add(d);
  } else {
    let set = new Set<string>();
    set.add(d);
    m.set(s,set);
  }   
}

rl.on('line',(line : string) => {
    let rel = line.split(')');
    console.assert(rel.length === 2);
    rel.forEach(v => names.add(v));
    addTo(map,rel[1], rel[0]);
    addTo(rmap,rel[0], rel[1]);
}); 

function getNumOrbits(p : string) : number {
  if(!map.has(p)) {
    return 0;
  }
  let s = map.get(p);
  let total = s.size;
  s.forEach(x => total += getNumOrbits(x));
  return total;
}

function partB() : number {
  console.assert(names.has('YOU'));
  console.assert(names.has('SAN'));

  let dist = new Map<string,number>();
  let queue = [ {loc: 'YOU', dist: 0} ];
  while(queue.length > 0) {
    let e = queue.shift();
    if(dist.has(e.loc)) {
      continue;
    }
    if(e.loc === 'SAN') {
      return e.dist;
    }
    dist.set(e.loc,e.dist);
    let neighbors = [];
    if(map.has(e.loc)) {
      map.get(e.loc).forEach(x => neighbors.push(x));
    }
    if(rmap.has(e.loc)) {
      rmap.get(e.loc).forEach(x => neighbors.push(x));
    }
    neighbors.forEach(x => {
      if(!dist.has(x)) {
        queue.push({loc: x, dist: e.dist+1});
      }
    });
  }
  return -1;
}

rl.on('close', () => {
  let list = [];
  names.forEach(x => list.push(x));
  let total = list.reduce((pv, n) => pv + getNumOrbits(n), 0);
  console.log(total);

  console.log(partB() - 2);
});