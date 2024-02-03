import React from 'react';
import styled from 'styled-components';
import { useUpload, useUploadFiles } from './UploadContext';
import UploadFileTile from './UploadFileTile';

const UploadsStyles = styled.div`
  width: 270px;
  height: 100vh;
  background: var(--primary-dark);
  display: grid;
  grid-template-rows: auto 1fr;
  overflow: hidden;

  #header {
    width: 100%;
    padding: 1rem;
    display: grid;
    grid-template-columns: 1fr auto 1fr;
    align-items: center;
    justify-items: center;
    box-sizing: border-box;
    font-weight: bolder;
    box-shadow: 0 0 12px 4px var(--primary-dark);
    z-index: 10;

    #clear {
      justify-self: start;
      margin-left: 0.5em;
      padding: 0;
      background: none;
      box-shadow: none;
      cursor: pointer;
    }
  }
`;

const UploadListStyles = styled.div`
  height: 100%;
  overflow-y: auto;
`;

export default function Uploads(): JSX.Element {
  const uploadFiles = useUploadFiles();
  const { clearCompleted } = useUpload();

  if (uploadFiles.length === 0) {
    return <></>;
  }

  return (
    <UploadsStyles>
      <div id="header">
        <button type="button" id="clear" onClick={clearCompleted}>
          ❌
        </button>
        <span>Uploads</span>
      </div>
      <UploadListStyles>
        {uploadFiles.map(file => (
          <UploadFileTile key={file.id} file={file} />
        ))}
      </UploadListStyles>
    </UploadsStyles>
  );
}
