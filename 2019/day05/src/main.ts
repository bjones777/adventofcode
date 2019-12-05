import * as readline from 'readline';
import { decode } from 'punycode';

let rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
  terminal: false
});

let input = 1;
function getInput(): number {
  return input;
}

let output = 0;
function setOutput(v: number) {
  console.log(v);
  output = v;
}

enum Instr {
  Add = 1,
  Multiply = 2,
  Input = 3,
  Output = 4,
  JumpIfTrue = 5,
  JumpIfFalse = 6,
  LessThan = 7,
  Equals = 8,
  Halt = 99
};
enum Mode { Position = 0, Immediate = 1 };
class DecodedInstr {
  public instr: Instr;
  public instrMode: Mode;
  public param1Mode: Mode;
  public param2Mode: Mode;
  public param3Mode: Mode;
}

function decodeInstr(n: number): DecodedInstr {
  let di = new DecodedInstr();
  let de = n % 100;
  let c = Math.floor(n / 100) % 10;
  let b = Math.floor(n / 1000) % 10;
  let a = Math.floor(n / 10000) % 10;
  di.param1Mode = (c === 0) ? Mode.Position : Mode.Immediate;
  di.param2Mode = (b === 0) ? Mode.Position : Mode.Immediate;
  di.param3Mode = (a === 0) ? Mode.Position : Mode.Immediate;

  let e = de % 10;
  if (e === Instr.Add) {
    di.instr = Instr.Add;
  } else if (e === Instr.Multiply) {
    di.instr = Instr.Multiply;
  } else if (e === Instr.Input) {
    di.instr = Instr.Input;
  } else if (e === Instr.Output) {
    di.instr = Instr.Output;
  } else if (e === Instr.JumpIfTrue) {
    di.instr = Instr.JumpIfTrue
  } else if (e === Instr.JumpIfFalse) {
    di.instr = Instr.JumpIfFalse;
  } else if (e === Instr.LessThan) {
    di.instr = Instr.LessThan;
  } else if (e === Instr.Equals) {
    di.instr = Instr.Equals;
  } else if (de == Instr.Halt) {
    di.instr = Instr.Halt;
  } else {
    throw new Error(`Invalid instrcution ${de}`);
  }
  if (di.instr !== Instr.Halt) {
    let d = Math.floor(de / 10);
    di.instrMode = (d === 0) ? Mode.Position : Mode.Immediate;
  } else {
    di.instrMode = Mode.Position;
  }
  return di;
}

function getValue(arr: number[], offset: number, mode: Mode): number {
  if (mode === Mode.Position) {
    return arr[arr[offset]];
  } else if (mode === Mode.Immediate) {
    return arr[offset];
  }
  throw new Error(`unknown mode ${mode}`);
}

function runStep(arr, ip) {
  let di = decodeInstr(arr[ip]);
  let opCode = di.instr;

  if (opCode === Instr.Add) {
    let v1 = getValue(arr, ip + 1, di.param1Mode);
    let v2 = getValue(arr, ip + 2, di.param2Mode);
    console.assert(di.param3Mode === Mode.Position);
    arr[arr[ip + 3]] = v1 + v2;
    return ip + 4;
  } else if (opCode === Instr.Multiply) {
    let v1 = getValue(arr, ip + 1, di.param1Mode);
    let v2 = getValue(arr, ip + 2, di.param2Mode);
    console.assert(di.param3Mode === Mode.Position);
    arr[arr[ip + 3]] = v1 * v2;
    return ip + 4;
  } else if (opCode === Instr.Input) {
    console.assert(di.param1Mode === Mode.Position);
    arr[arr[ip + 1]] = getInput();
    return ip + 2;
  } else if (opCode === Instr.Output) {
    let v1 = getValue(arr, ip + 1, di.param1Mode);
    setOutput(v1);
    return ip + 2;
  } else if (opCode === Instr.JumpIfTrue) {
    let v1 = getValue(arr, ip + 1, di.param1Mode);
    let v2 = getValue(arr, ip + 2, di.param2Mode);
    return v1 !== 0 ? v2 : ip + 3;
  } else if (opCode === Instr.JumpIfFalse) {
    let v1 = getValue(arr, ip + 1, di.param1Mode);
    let v2 = getValue(arr, ip + 2, di.param2Mode);
    return v1 === 0 ? v2 : ip + 3;
  } else if (opCode === Instr.LessThan) {
    let v1 = getValue(arr, ip + 1, di.param1Mode);
    let v2 = getValue(arr, ip + 2, di.param2Mode);
    console.assert(di.param3Mode === Mode.Position);
    arr[arr[ip + 3]] = v1 < v2 ? 1 : 0;
    return ip + 4;
  } else if (opCode === Instr.Equals) {
    let v1 = getValue(arr, ip + 1, di.param1Mode);
    let v2 = getValue(arr, ip + 2, di.param2Mode);
    console.assert(di.param3Mode === Mode.Position);
    arr[arr[ip + 3]] = v1 === v2 ? 1 : 0;
    return ip + 4;
  }
  else if (opCode === Instr.Halt) {
    return -1;
  }
  throw new Error(`Unknown opCode ${opCode} at ip ${ip}, ${arr}`);
}

function runSim(arr) {
  let ip = 0;
  for (; ;) {
    ip = runStep(arr, ip);
    if (ip < 0) {
      return arr[0];
    }
  }
}

let arr = []
rl.on('line', (line: string) => {
  let instr = line.split(',').map(x => parseInt(x, 10));
  instr.forEach(i => arr.push(i));
});

rl.on('close', () => {
  console.log("Part A");
  input = 1;
  runSim(arr.slice());

  console.log("Part B")
  input = 5;
  runSim(arr.slice());
});