type Color = { r: number, g: number, b: number };
type Callback = (err: Error | undefined, colors: Color[]) => void;

declare function octree(file: string, maxColors: number, callback?: Callback, frameNumber?: number): void;
declare function octree(file: string, callback?: Callback, frameNumber?: number): void;

declare function minDiff(file: string, palette: Color[], callback?: Callback, frameNumber?: number): void;
declare function minDiff(file: string, callback?: Callback, frameNumber?: number): void;

declare function mixed(file: string, firstStepMaxColors: number, palette: Color[], callback?: Callback, frameNumber?: number): void;
declare function mixed(file: string, firstStepMaxColors: number, callback?: Callback, frameNumber?: number): void;
declare function mixed(file: string, palette: Color[], callback?: Callback, frameNumber?: number): void;
declare function mixed(file: string, callback?: Callback, frameNumber?: number): void;

export = {
  octree,
  minDiff,
  mixed,

  octreeGet: octree,
  minDiffGet: mindiff,
  mixedGet: mixed,
};
