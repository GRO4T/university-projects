import axios, { AxiosError } from 'axios';
import React, { useState } from 'react';
import { bytesToHumanReadable, dateToHumanReadable } from '../utils/utils';
import { useToast } from './ToastContext';
import { DriveElementStyles, DriveElementActionBtnStyles } from '../styles/DriveElementStyles';

interface DriveFileProps {
  file: DriveFile;
  deleteFile: () => void;
}

export default function DriveFile({ file, deleteFile }: DriveFileProps): JSX.Element {
  const [deleteState, setDeleteState] = useState<FetchState>('idle');

  const launchToast = useToast();

  const handleDownload = () => window.location.assign(`/api/file/${file.id}`);

  const handleDelete = () => {
    if (deleteState !== 'pending') {
      setDeleteState('pending');
      axios
        .delete(`/api/file/${file.id}`)
        .then(deleteFile)
        .catch((err: AxiosError) => {
          setDeleteState('failed');
          launchToast('error', err.message);
        });
    }
  };

  const isBeingDeleted = () => deleteState === 'pending';

  return (
    <DriveElementStyles className={isBeingDeleted() ? 'deleting' : ''}>
      <td>📄</td>
      <td className="left">{file.filename}</td>
      <td className="right">{bytesToHumanReadable(file.size)}</td>
      <td>{dateToHumanReadable(file.lastModified)}</td>
      <td>
        <DriveElementActionBtnStyles
          type="button"
          disabled={isBeingDeleted()}
          onClick={handleDownload}
        >
          📥
        </DriveElementActionBtnStyles>
        <DriveElementActionBtnStyles
          type="button"
          disabled={isBeingDeleted()}
          onClick={handleDelete}
          className="danger"
        >
          ♻️
        </DriveElementActionBtnStyles>
      </td>
    </DriveElementStyles>
  );
}
