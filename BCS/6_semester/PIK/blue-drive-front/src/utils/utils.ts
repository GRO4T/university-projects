export const bytesToHumanReadable = (bytes: number): string => {
  const units = ['B', 'kB', 'MB', 'GB'];
  const precision = 3;

  if (bytes <= 0) {
    return '0 B';
  }

  const orderOfMagnitude = Math.floor(Math.log2(bytes) / 10);
  const converted = bytes / 1024 ** orderOfMagnitude;
  const size = Math.floor(Math.log10(converted)) + 1;
  const actualPrecision = 10 ** (precision - size);
  const rounded = Math.round(converted * actualPrecision) / actualPrecision;

  return `${rounded} ${units[orderOfMagnitude]}`;
};

export const dateToHumanReadable = (date: string): string => {
  const options: Intl.DateTimeFormatOptions = {
    dateStyle: 'long',
    timeStyle: 'short',
  };

  return new Date(date).toLocaleString(undefined, options);
};

export const fractionToPercentage = (num: number): string => `${Math.round(num * 100)}%`;

export const isValidDirName = (name: string): boolean => {
  const invalidNames = ['', '.', '..'];

  if (invalidNames.includes(name) || name.includes('/')) {
    return false;
  }
  return true;
};

export const throwNoProvider = (name: string): void => {
  throw Error(`${name}Context has no Provider!`);
};
