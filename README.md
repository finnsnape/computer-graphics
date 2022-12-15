# TODO
## Main tasks
- Specular lighting (in progress)
- ~Fix lookAt~ -- just need to implement translate, rotate, lookAt ourselves!
- Export of image files and creation of animated video
- Mirrors (in progress)
- Test on vagrant
## General
- Modes 7 and 8 seem to be functioning the same at the moment
- Try moving model (or part of model, e.g., just one box!)
- Add comments
- Potentially split into more classes
- Fix rounding errors and type warnings
- Reduce duplicate functions (e.g., transposing)
- Improve function names
- Potentially add own implementations of trivial functions (rotate, perspective)
- Consider texture mapping
- Fix proximity lighting to work when light is very close to intersection
- Find way to use perspective logic in our demo
- Remove unused imports
- Make light translation better, like camera translation

## Notes
- negative z is away from us
- a * b in glm is same as a * b in maths (for matrices)

# Computer Graphics
## Description
Program that implements both rasterised and ray traced rendering methods.

Also features more complex techniques such as:
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
