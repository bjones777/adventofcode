import * as readline from 'readline';

// 330806 - too low
let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

  class Reactant {
    public Element : string;
    public Ratio : number;

    constructor(name : string,ratio : number) {
      this.Element = name;
      this.Ratio = ratio;
    }
  }

function createReactant(s : string) {
  s = s.trim();
  let components = s.split(' ');
  console.assert(components.length === 2);
  return new Reactant(components[1],parseInt(components[0],10));
}

let outputs = new Map<string,Reactant[]>()

rl.on('line',(line : string) => {
    let inOut = line.split('=>');
    console.assert(inOut.length === 2);
    let inputs = inOut[0];
    let output = inOut[1].trim();
    let reactants : Reactant[] = [];
    reactants.push(createReactant(output));
    reactants.push.apply(reactants,inputs.split(',').map((s) => createReactant(s)));
    console.assert(!outputs.has(reactants[0].Element));
    outputs.set(reactants[0].Element,reactants);
}); 

let leftOvers = new Map<string,number>();

function getOreFor(amount : number,element : string) : number {
  if(element === "ORE") {
    let ready = leftOvers.get(element);
    if(ready < amount) {
      throw new Error("Ran out of ore");
    }
    leftOvers.set(element,ready-amount);
    return amount;
  }
  if(!outputs.has(element)) {
    throw new Error(`${element} not found in inputs`);
  }
  let ready = leftOvers.get(element);
  if(ready == null) {
    ready = 0;
  }
  if(ready >= amount) {
    leftOvers.set(element,ready - amount);
    return 0;
  }
  amount -= ready;
  leftOvers.set(element,0);
  let reactants = outputs.get(element);
  let ratio = Math.ceil(amount/reactants[0].Ratio);
  let leftOver = ratio * reactants[0].Ratio - amount;
  let oreFor = 0;
  for (let i = 1;i < reactants.length;++i) {
    oreFor += getOreFor(ratio*reactants[i].Ratio,reactants[i].Element);
  }
  if(leftOver > 0) {
    if(leftOvers.has(element)) {
      leftOvers.set(element,leftOvers.get(element) + leftOver);
    } else {
      leftOvers.set(element,leftOver);
    }
  }
  return oreFor;
}

function partA() {
  leftOvers = new Map<string,number>();
  leftOvers.set("ORE",1000000000000);
  console.log(getOreFor(1,"FUEL"));
}

function partB() {
  let low = 0n;
  let high = 1000000000000n;
  while(low < high) {
    leftOvers = new Map<string,number>();
    leftOvers.set("ORE",1000000000000);
    let mid = (high + low)/2n;
    try {
      getOreFor(Number(mid.toString()),"FUEL");
      low = mid+1n;
    } catch(e) {
      high = mid;
    }
  }
  console.log((low-1n).toString());
}

rl.on('close',() => {
  partA();
  partB();
});