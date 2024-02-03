import React, { useEffect, useState } from 'react';
import styled from 'styled-components';
import DriveFile from './DriveFile';
import DriveDirectory from './DriveDirectory';
import { driveElementComparisonFn, strArrToPath } from '../utils/driveUtils';

const DriveFileTableStyles = styled.table`
  --emoji-clearance: 4rem;

  align-self: flex-start;
  width: 100%;
  font-size: 1.5rem;

  th {
    padding: 1rem 0;
    color: var(--primary);
    background-color: var(--darkest);
  }

  tr:not(:first-child):hover {
    background-color: var(--darker);
  }

  td {
    padding: 0.5rem 0;
    text-align: center;
    border-bottom: 2px solid var(--darker);
  }

  [aria-label='Directory'] {
    width: var(--emoji-clearance);
  }

  .left {
    text-align: left;
  }

  .right {
    text-align: right;
  }
`;

interface DriveFileTableProps {
  path: string[];
  files: DriveElement[];
  goToDir: (dirname: string) => void;
  goToParentDir: () => void;
  deleteFileFactory: (id: string) => () => void;
}

export default function DriveFileTable({
  path,
  files,
  goToDir,
  goToParentDir,
  deleteFileFactory,
}: DriveFileTableProps): JSX.Element {
  const [currentFiles, setCurrentFiles] = useState<DriveElement[]>([]);

  useEffect(() => {
    const currentPath = strArrToPath(path);
    const filtered = files.filter(f => f.directoryPath === currentPath);
    if (path.length) {
      filtered.push({ type: 'directory', id: '..', dirname: '..', directoryPath: '' });
    }
    filtered.sort(driveElementComparisonFn);
    setCurrentFiles(filtered);
  }, [path, files]);

  return (
    <DriveFileTableStyles cellSpacing="0">
      <tbody>
        <tr>
          <th aria-label="Directory">{}</th>
          <th className="left">Name</th>
          <th className="right">Size</th>
          <th>Modified</th>
          <th>Actions</th>
        </tr>
        {currentFiles.map(f =>
          f.type === 'file' ? (
            <DriveFile key={f.id} file={f} deleteFile={deleteFileFactory(f.id)} />
          ) : (
            <DriveDirectory
              key={f.id}
              directory={f}
              deleteDirectory={deleteFileFactory(f.id)}
              goToParentDir={goToParentDir}
              goToDir={goToDir}
            />
          )
        )}
      </tbody>
    </DriveFileTableStyles>
  );
}
