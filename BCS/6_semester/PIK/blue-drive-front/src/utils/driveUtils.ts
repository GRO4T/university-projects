export const strArrToPath = (path: string[]): string => '/'.concat(path.join('/'));

export const strArrToJoinablePath = (path: string[]): string =>
  path.length ? strArrToPath(path) : '';

export const dirNormalizer = (el: DriveElement): DriveElement =>
  el.type === 'directory'
    ? {
        ...el,
        dirname: el.directoryPath.split('/').slice(-1)[0],
        directoryPath: strArrToPath(el.directoryPath.split('/').slice(1, -1)),
      }
    : el;

export const driveElementComparisonFn = (a: DriveElement, b: DriveElement): number => {
  if (a.type === b.type) {
    const nameA = a.type === 'directory' ? a.dirname : a.filename;
    const nameB = b.type === 'directory' ? b.dirname : b.filename;
    return nameA.localeCompare(nameB);
  }
  return a.type === 'directory' ? -1 : 1;
};
