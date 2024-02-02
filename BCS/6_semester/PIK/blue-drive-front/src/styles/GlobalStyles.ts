import { createGlobalStyle } from 'styled-components';

const GlobalStyles = createGlobalStyle`
  :root{
    /* Blue 500 */
    --primary: #2196F3;
    --primary-dark: #1976D2;
    --complementary: #f37f21;
    --triadic1: #8021f3;
    --triadic2: #f32194;

    --white: #fff;
    --black: #000;

    /* Discord bg */
    --dark: #36393f;
    --darker: #2f3136;
    --darkest: #202225;

    --secondary: #546E7A;
    --success: #43A047;
    --danger: #E53935;
    --warning: #FB8C00;
    --info: #039BE5;

    --secondary-dark: #37474F;
    --success-dark: #2E7D32;
    --danger-dark: #C62828;
    --warning-dark: #EF6C00;
    --info-dark: #0277BD;

    --shadow-color: rgba(0, 0, 0, 0.8);

    --triadic1-hue: #8021f355;
  }

  html{
    height: 100vh;
    background-color: var(--dark);
  }
  
  button{
    --btn-color: var(--primary);
    --btn-color-hover: var(--primary-dark);
    --btn-shadow-color: var(--primary);
    border: none;
    outline: none;

    padding: 0.75rem;
    color: var(--white);
    background: var(--btn-color);
    border-radius: 0.5rem;
    box-shadow: 0 0 8px var(--btn-shadow-color);
    transition: background-color 0.2s, box-shadow 0.2s;

    &.secondary{
      --btn-color: var(--secondary);
      --btn-color-hover: var(--secondary-dark);
    }

    &.success{
      --btn-color: var(--success);
      --btn-color-hover: var(--success-dark);
    }
    
    &.danger{
      --btn-color: var(--danger);
      --btn-color-hover: var(--danger-dark);
    }

    &.warning{
      --btn-color: var(--warning);
      --btn-color-hover: var(--warning-dark);
    }

    &.info{
      --btn-color: var(--info);
      --btn-color-hover: var(--info-dark);
    }

    &:hover{
      background: var(--btn-color-hover);
    }

    &:active{
      --btn-shadow-color: var(--triadic1);
    }

    &:disabled{
      background: var(--btn-color);
      filter: brightness(0.6) contrast(0.8);
    }
  }

  input{
    line-height: 1em;
    color: var(--white);
    border: none;
    box-sizing: border-box;
    outline: none;
    transition: border 0.2s;

    &:hover {
      --input-border-color: var(--primary-dark);
    }

    &:focus {
      --input-border-color: var(--primary);
    }
  }

  .card{
    margin: 2rem;
    padding: 4rem;
    background-color: var(--darker);
    box-shadow: 4px 4px 16px var(--shadow-color);
    border-radius: 6px;
  }
`;

export default GlobalStyles;
