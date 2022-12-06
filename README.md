# RedNoise
## Status
- W6T7 (complete)
## Todo
- ~Fix x orbit~
- ~Fix ray tracing orbit (need to utilise orientation)~
- ~Ensure we are translating correctly~
- Add comments
- ~~Figure out global WIDTH, HEIGHT~~
- Potentially split into more classes
- Try fix event being missed (CTRL+C sometimes won't exit)
- ^ Events sometimes not registered, maybe we need &window? or \*/\*\*
- Fix rounding errors and type warnings
- Check our depth buffer is x by y and not y by x
- Separate what's required (~e.g., not all types of camera need a depth buffer~)
- Reduce duplicate functions (e.g., transposing)
- Improve function names
- Potentially add own implementations of trivial functions (rotate, perspective)
- Consider texture mapping
- Test on vagrant
- ~lookAt~
- Ensure lookAt works after translations, rotations etc. Should investigate glm::lookAt

## Notes
- negative z is away from us
- a * b in glm is same as a * b in maths (for matrices)