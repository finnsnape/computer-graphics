# TODO
## Main tasks
- Specular lighting (in progress)
- Export of image files and creation of animated video
- Mirrors (in progress, see below)
- Test on vagrant
## General
- Make lighting more realistic (circles should join up)
- Are we sure our reflection angles are correct for the mirror? e.g., ceiling, seems not to be reflecting as "early" as it should
- Try moving model (or part of model, e.g., just one box!)
- Stretch model?
- Add comments
- Potentially split into more classes
- Fix rounding errors and type warnings
- Reduce duplicate functions (e.g., transposing)
- Improve function names
- Consider texture mapping
- Fix proximity lighting to work when light is very close to intersection
- Find way to use perspective logic in our demo
- Remove unused imports

## Notes
- negative z is away from us
- a * b in glm is same as a * b in maths (for matrices)

# Computer Graphics
## Description
Program that implements both rasterised and ray traced rendering methods.

Also features more complex techniques such as:
- Translation, rotation and lookAt
- Shadows 
- Proximity lighting 
- Angle of incidence lighting 
- Ambient lighting 
- Specular lighting 
- Mirrors

## Requirements
- `clang++`
- `sdl2`

## Running
- `make`
