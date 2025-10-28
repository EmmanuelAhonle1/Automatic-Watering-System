export const COLORS = {
  PRIMARY_TAN: "#D2B48C",
  CREAM: "#F5F5DC",
  TEXT_GRAY: "#555555",
  PLANT_GREEN: "#90A955",
  DARK_TAN: "#C4A484",
};

// CSS Variables - can be injected into your root stylesheet
export const cssVariables = `
  :root {
    --color-primary-tan: ${COLORS.PRIMARY_TAN};
    --color-cream: ${COLORS.CREAM};
    --color-text-gray: ${COLORS.TEXT_GRAY};
    --color-plant-green: ${COLORS.PLANT_GREEN};
    --color-dark-tan: ${COLORS.DARK_TAN};
  }
`;
