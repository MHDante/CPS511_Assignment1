I'm very sorry for the delay in handing this in.

I'm sure I could've Handed in a sufficient project a month ago,
but I became lost inside the complexities that this project could entail.
This is the first time that I've used C++, and I became obsessed with
learning it at depth. I reshaped the entire codebase provided to this purpose.
Furthermore, I decided to derive and develop a lot of the math myself,
admittedly getting lost in some of the hairier parts. (Converting a ray to 
object space)

I dont expect full marks (or any marks at all) I just wanted to maximize 
the learning that I could derive from the assignment.

To compile, open project in VS2015.


Matrices:
glTranslatef(center.x, center.y, center.z);
[1	0	0	center.x]
[0	1	0	center.y]
[0	0	1	center.z]
[0	0	0	1		]

glRotatef(angle, 0, 1, 0);
[cos(angle)		0		sin(angle)	0]
[0				1		0			0]
[-sin(angle)	0		cos(angle)	0]
[0				0		0			1]

glScalef(dim.x/2, dim.y/2, dim.z/2);
[dim.x/2	0			0		0]
[0			dim.y/2		0		0]
[0			0			dim.z/2	0]
[0			0			0		1]