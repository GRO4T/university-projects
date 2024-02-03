// This file is used to hold ambient type declarations, as well as type shims
// for npm module without type declarations, and assets files.

// For example, to shim modules without declarations, use:
// declare module "package-without-declarations"

// And to shim assets, use (one file extension per `declare`):
// declare module "*.png"

type FetchState = 'idle' | 'pending' | 'success' | 'failed';

interface DriveFile {
  type: 'file';
  id: string;
  filename: string;
  directoryPath: string;
  lastModified: string;
  size: number;
  contentType: string;
}

interface DriveDirectory {
  type: 'directory';
  id: string;
  dirname: string;
  directoryPath: string;
}

type DriveElement = DriveFile | DriveDirectory;

interface UploadFile {
  id: number;
  name: string;
  path: string;
  status: FetchState;
  loaded: number;
  total: number;
  cancelTokenSource: import('axios').CancelTokenSource;
}

declare namespace Intl {
  interface DateTimeFormatOptions {
    dateStyle?: 'full' | 'long' | 'medium' | 'short';
    timeStyle?: 'full' | 'long' | 'medium' | 'short';
  }
}
