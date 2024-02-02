const importResolverSettings = {
  'import/resolver': {
    node: {
      extensions: ['.js', '.jsx', '.ts', '.tsx'],
    },
  },
};

const eslintRules = {
  'no-console': 'warn',
  'no-void': 'off',
  'import/no-extraneous-dependencies': ['error', { devDependencies: ['sonarqube-scanner.js'] }],
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
  overrides: [
    {
      files: ['**/*.ts', '**/*.tsx'],
      parser: `@typescript-eslint/parser`,
      parserOptions: {
        project: './tsconfig.json',
      },
      extends: [
        'wesbos',
        'eslint:recommended',
        'plugin:@typescript-eslint/recommended',
        'plugin:@typescript-eslint/recommended-requiring-type-checking',
      ],
      plugins: ['@typescript-eslint'],
      rules: {
        'no-use-before-define': 'off',
        '@typescript-eslint/no-use-before-define': ['error'],
        'react/jsx-filename-extension': [1, { extensions: ['.ts', '.tsx'] }],
        ...eslintRules,
        ...prettierRules,
      },
      settings: { ...importResolverSettings },
    },
  ],
};
