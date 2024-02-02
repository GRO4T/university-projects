import styled from 'styled-components';

export const DriveElementStyles = styled.tr`
  &.directory {
    cursor: pointer;
  }

  td {
    transition: background-color 0.2s;
  }

  &.deleting td {
    background-color: var(--danger);
  }
`;

export const DriveElementActionBtnStyles = styled.button`
  --btn-color: var(--darkest) !important;
  --btn-shadow-color: var(--darker);

  width: var(--emoji-clearance);
  margin: 0 0.5rem;

  &.hidden {
    visibility: hidden;
  }
`;
