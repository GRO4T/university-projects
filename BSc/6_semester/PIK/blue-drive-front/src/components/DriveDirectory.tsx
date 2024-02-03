import axios, { AxiosError } from 'axios';
import React, { useState } from 'react';
import { DriveElementActionBtnStyles, DriveElementStyles } from '../styles/DriveElementStyles';
import { useToast } from './ToastContext';

interface DriveDirectoryProps {
  directory: DriveDirectory;
  deleteDirectory: () => void;
  goToParentDir: () => void;
  goToDir: (dirname: string) => void;
}

export default function DriveDirectory({
  directory: { id, dirname },
  deleteDirectory,
  goToParentDir,
  goToDir,
}: DriveDirectoryProps): JSX.Element {
  const [deleteState, setDeleteState] = useState<FetchState>('idle');

  const launchToast = useToast();

  const handleDelete = (e: React.MouseEvent) => {
    if (deleteState !== 'pending') {
      setDeleteState('pending');
      axios
        .delete(`/api/directory/${id}`)
        .then(deleteDirectory)
        .catch((err: AxiosError) => {
          setDeleteState('failed');
          launchToast('error', err.message);
        });
    }
    e.stopPropagation();
  };

  const isBeingDeleted = () => deleteState === 'pending';

  const isParent = dirname === '..';

  const handleClick = () => (isParent ? goToParentDir() : goToDir(dirname));

  return (
    <DriveElementStyles
      className={`directory ${isBeingDeleted() ? 'deleting' : ''}`}
      onClick={handleClick}
    >
      <td>📁</td>
      <td className="left">{dirname}</td>
      <td />
      <td />
      <td>
        <DriveElementActionBtnStyles
          type="button"
          className={`danger ${isParent ? 'hidden' : ''}`}
          disabled={isBeingDeleted() || isParent}
          onClick={handleDelete}
        >
          ♻️
        </DriveElementActionBtnStyles>
      </td>
    </DriveElementStyles>
  );
}
