/* eslint-disable camelcase */
/* eslint-disable react/prop-types */
import styled from 'styled-components';

const TileStyled = styled.div`
  --green: #8bc34a;
  --red: #f44336;

  padding: 1rem;
  background-color: rgba(0, 0, 0, 0.1);
  border: 1px solid black;
  border-radius: 1rem;
  display: grid;
  grid-template-columns: auto 4px;
  grid-template-rows: 1fr 1fr;
  gap: 1rem;
  overflow: hidden;

  #info {
    grid-row: 1 / 3;
    display: grid;
    grid-template-columns: auto 1fr;
    grid-template-rows: auto 1fr;
    gap: 1rem;
    align-items: end;
  }

  &.active {
    background-color: #2196f330;
  }

  .True {
    color: var(--green);
  }

  .False {
    color: var(--red);
  }

  .pending {
    color: #fff3;
  }

  .correct {
    background-color: var(--green);
  }

  .incorrect {
    background-color: var(--red);
  }
`;

const boolToStr = state => (state ? 'True' : 'False');

const predictionToStr = state => {
  if (state === 'pending') {
    return state;
  }
  return state ? 'False' : 'True';
};

const getPredictionCorrectnessClass = (prediction, active, purchased) => {
  if (prediction === 'pending' || active) {
    return '';
  }
  return prediction === purchased ? 'correct' : 'incorrect';
};

export function Tile({ data }) {
  const { id, gender, city, active, purchased, prediction_A, prediction_B, event_cnt } = data;

  const activeStr = boolToStr(active);
  const purchasedStr = boolToStr(purchased);
  const predAStr = predictionToStr(prediction_A);
  const predBStr = predictionToStr(prediction_B);

  const ACorrectClass = getPredictionCorrectnessClass(prediction_A, active, purchased);
  const BCorrectClass = getPredictionCorrectnessClass(prediction_B, active, purchased);

  return (
    <TileStyled className={active && 'active'}>
      <div id="info">
        <div id="id">{id}</div>
        <div id="personal">
          {gender === 'M' ? 'Male' : 'Female'} from {city}
        </div>
        <div>
          <div>
            <span>Events: </span>
            <span>{event_cnt}</span>
          </div>
          <div>
            <span>Active: </span>
            <span className={activeStr}>{activeStr}</span>
          </div>
          <div>
            <span>Purchased: </span>
            <span className={purchasedStr}>{purchasedStr}</span>
          </div>
        </div>
        <div>
          <div>
            <span>Needs help A? </span>
            <span className={predAStr}>{predAStr}</span>
          </div>
          <div>
            <span>Needs help B? </span>
            <span className={predBStr}>{predBStr}</span>
          </div>
        </div>
      </div>
      <div className={ACorrectClass} />
      <div className={BCorrectClass} />
    </TileStyled>
  );
}
