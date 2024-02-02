module.exports = {
  siteMetadata: {
    title: 'BlueDrive',
  },
  plugins: [
    'gatsby-plugin-image',
    'gatsby-plugin-react-helmet',
    'gatsby-plugin-sharp',
    'gatsby-plugin-styled-components',
    'gatsby-transformer-sharp',
    {
      resolve: 'gatsby-source-filesystem',
      options: {
        name: 'images',
        path: './src/assets/images/',
      },
      __key: 'images',
    },
  ],
  proxy: {
    prefix: '/api',
    url: 'http://localhost:8095',
  },
};
