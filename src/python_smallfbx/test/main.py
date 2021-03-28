import sys
import smallfbx as sfbx


def main():
	filepath = sys.argv[1]

	d = sfbx.Document(filepath)
	print(f'FBX valid: {d.valid()}')

	if not d.valid():
		print(f'NOOOOOO!')
		sys.exit(2)

	print(f'FBX Version: {d.version()}')
	stacks = d.animation_stacks()
	print(f'Animation Stacks: {stacks}')
	for s in stacks:
		print(f'{s.name()}[{s.full_name()}] @ {s.id()}')
		for l in s.animation_layers():
			print(f'{l.name()}[{l.full_name()}] @ {l.id()}')


if __name__ == '__main__':
	main()