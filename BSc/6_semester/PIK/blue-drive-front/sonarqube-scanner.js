const scanner = require('sonarqube-scanner');

scanner(
  {
    serverUrl: 'http://sonarqube:9000',
    options: {
      'sonar.projectName': 'blue-drive-front',
      'sonar.projectKey': 'blue-drive-front',
      'sonar.sourceEncoding': 'UTF-8',
      'sonar.sources': './src',
    },
  },
  () => process.exit()
);
