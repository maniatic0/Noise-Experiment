# Notes

Summary from https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/procedural-patterns-noise-part-1/simple-pattern-examples , just to remember it.

## Terminology

The technique of **summing up layers** of noise which frequency and amplitude are related to each other, can be called a **fractal sum**.

When successive layers of a fractal noise have an amplitude which is inversely proportional to their frequency, the term used to describe the result is **pink noise**.

The change of frequency and amplitude between successive layers almost forms the signature of the result noise curve. It defines its **spectral properties**. We use the term **spectral densities** to define the various **frequencies (layers)** the resulting noise is made of. And each one of these layers has a specific **amplitude** which we call **power spectra**. Amplitude and frequency can either be related to each other like in the case of pink noise or not. You could also have a relation in the change of frequency between successive layers.

The use of the word noise in the name (pink noise) might be a bit misleading as it refers to a sum of noise functions with correlated frequencies and amplitudes. The term **octave** is also sometimes (mis-)used in place of the word **layer**. The term **layer** is more generic than **octave** which is also used in music. An **octave** is a **doubling** or **halving** of a **frequency**. If it is used in a program (or in literature) it should mean that each successive layer in the computation of a fractal sum is twice the frequency of the previous layer. It means that the term change of rate in our equation would take the value 2. If the frequency ratio between successive layers is different than 2, the use of this term is inaccurate and layer should be used instead. When we double the frequency between layers and that the amplitude of these layers is inversely proportional to their frequency, we obtain a special type of pink noise which we call **Brownian noise** (named after the mathematician Robert Brown).

In computer graphics, you will often find that fractal functions are called **fBm** (which stands for **fractional brownian motion**). The CG community has borrowed most of these terms from the mathematics field, mainly as a convenient way of labelling functions which are using these techniques in a generally simplified/simpler form. In the generic form of the **fBm** function, the amplitude of a layer doesn't have to be inversionaly proportional to its frequency. You can use two different values to control how the frequency and the amplitude change between layers. The word **lacunarity** is used to control the rate by which the frequency changes from layer to layer. **Lacunarity** has a special meaning in in the field of fractal (check the lesson on fractals from more information). There is no special word for the rate of change in the amplitude from layer to layer, but we will be using **gain**.

### Fractal Sum

Our second example is a demonstration of the fractal sum that we have already explained for the 1D case. We accumulate the contribution of five layers of noise. Between each successive layer we multiply the frequency of the point from the previous layer by two and divide the amplitude from the previous layer by two.

Note that because we sum up several layers of noise, the result could be greater than 1 which will be a problem when we will convert this value to a pixel color. We can clamp the value when it is converted to a pixel color, but a better solution is to normalize the array of noise values by dividing all the values in the array by the maximum computed value. 

In the code you can experiment by changing the multiplier for the frequency and the amplitude, turning your fractal noise function into a more generic fBm function which we have described earlier on.

### Turbulence

Turbulence is a function built on the same principle as the fractal sum. However instead of using the noise function directly for each layer, we will use the absolute value of the signed noise. We will first convert the result of the noise into a signed noise, and then take the absolute value of this result. As you can see in the following figure, processing the noise function that way, creates a profile that seems to be made of bumps. Wherever the curve is negative (black line) we will mirror the curve in these areas along the x axis. The red line curve is the result. Using this technique with a 2D noise can produce patterns suitable to simulate fire, smoke or clouds.

### Marble Texture

A marble texture can created by modulating the phase of sine pattern with a noise function or a fractal sum. The idea here is not to use the noise function directly to create the pattern but to perturb the function we are using to create the pattern. In that case, we perturb or shift the phase of the sine function with a fractal sum. This idea can be use to introduce randomness in any periodic or regular function. Our example is quite simple and only draw a black and white marble texture. However by introducing some color in the mix, it might be possible to create more realistic patterns.

### Wood Texture

Like the marble texture, the wood texture relies on a very simple trick. The idea is to multiply the noise function by a certain value greater than 1. Let's call the result of this multiplication g (historically it was called g in reference to wood grain). The texture is obtained by subtracting g from its integer part. Casting a positive float number to an integer will result in an integer necessarily smaller or equal to g. The result of that subtraction is therefore necessarily in the range [0:1) (1 exclusive). Figure 6 illustrates the process. In this example, we have multiplied the noise function by 4. The blue curve represents the value g, while the red curve represents the result of subtracting g from its integer part. Multiplying the noise function by a value greater than 4 would result in more breakups in the red curve. In 2D, these breakups mark the boundary between regions of lighter and darker color.