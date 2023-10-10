#!/usr/bin/env node

const amount = 65536;
const maxValue = 100;
const minValue = -100;


const { writeFileSync } = require('fs');

function generateValues(min, max, parser) {
  return Array.from({length: amount}, () => {
    const value = Math.random() * (max - min) + min;
    return parser(value);
  });
}

const valuesInt = generateValues(minValue, maxValue, Number.parseInt);
const valuesFloat = generateValues(minValue, maxValue, v => {
  return Number.parseFloat(v).toFixed(2);
});

const header = `#ifndef DATASET_H
#define DATASET_H
#define iDataset {${valuesInt.join(", ")}};
#define fDataset {${valuesFloat.join(", ")}};
#endif // DATASET_H
`;

try {
  writeFileSync('./Dataset.h', header);
} catch (err) {
  console.error(err);
}