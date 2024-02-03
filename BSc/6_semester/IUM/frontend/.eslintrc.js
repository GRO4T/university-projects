const importResolverSettings = {
  'import/resolver': {
    node: {
      extensions: ['.js', '.jsx', '.ts', '.tsx'],
    },
  },
};

const eslintRules = {
  'no-console': 'warn',
};

const prettierRules = {
  'prettier/prettier': [
    'error',
    {
      trailingComma: 'es5',
      singleQuote: true,
      printWidth: 100,
      endOfLine: 'auto',
      arrowParens: 'avoid',
    },
  ],
};

module.exports = {
  extends: ['wesbos'],
  rules: { ...eslintRules, ...prettierRules },
  settings: { ...importResolverSettings },
};
