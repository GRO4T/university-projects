export const getNewUIDGenerator = (): (() => number) => {
  let lastID = -1;
  return () => {
    lastID += 1;
    return lastID;
  };
};
