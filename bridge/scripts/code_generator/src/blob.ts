import fs from 'fs';
import {ClassObject} from "./declaration";

export class Blob {
  raw: string;
  dist: string;
  source: string;
  filename: string;
  objects: ClassObject[];

  constructor(source: string, dist: string, filename: string) {
    this.source = source;
    this.raw = fs.readFileSync(source, {encoding: 'utf-8'});
    this.dist = dist;
    this.filename = filename;
  }
}
