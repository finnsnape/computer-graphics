# RedNoise
## Status
- W7T4 (in progress)
## Tasks
- Specular lighting
- lookAt (0,0,0) as a key
- Export of image files and creation of animated video
## Fixes
- Try moving model (or part of model, e.g., just one box!)
- ~Fix x orbit~
- ~Fix ray tracing orbit (need to utilise orientation)~
- ~Ensure we are translating correctly~
- Add comments
- ~~Figure out global WIDTH, HEIGHT~~
- Potentially split into more classes
- Try fix event being missed (CTRL+C sometimes won't exit)
- ^ Events sometimes not registered, maybe we need &window? or \*/\*\*
- Fix rounding errors and type warnings
- ~Check our depth buffer is x by y and not y by x~
- Separate what's required (~e.g., not all types of camera need a depth buffer~)
- Reduce duplicate functions (e.g., transposing)
- Improve function names
- Potentially add own implementations of trivial functions (rotate, perspective)
- Consider texture mapping
- Test on vagrant
- ~lookAt~
- Ensure lookAt works after translations, rotations etc. Should investigate glm::lookAt
- Fix proximity lighting to work when light is very close to intersection
- Find way to use perspective logic in our demo
- Remove unused imports

## Notes
- negative z is away from us
- a * b in glm is same as a * b in maths (for matrices)