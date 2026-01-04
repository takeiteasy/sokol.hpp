#!/usr/bin/env python3
"""
Generate sokol.hpp using the sokol bindgen framework.

This script runs the C++ code generator to create sokol.hpp from the sokol headers.
"""
import sys
import os

# Add sokol/bindgen to Python path
sokol_bindgen_path = os.path.join(os.path.dirname(__file__), '..', 'sokol', 'bindgen')
sys.path.insert(0, sokol_bindgen_path)

import gen_cpp

def main():
    """Generate sokol.hpp from sokol headers"""

    # Change to bindgen directory for relative paths to work
    original_dir = os.getcwd()
    os.chdir(sokol_bindgen_path)

    try:
        # Define tasks (same as in gen_all.py)
        tasks = [
            ['../sokol_log.h',    'slog_',   []],
            ['../sokol_gfx.h',    'sg_',     []],
            ['../sokol_app.h',    'sapp_',   []],
            ['../sokol_time.h',   'stm_',    []],
            ['../sokol_audio.h',  'saudio_', []],
        ]

        # Output path
        output_path = os.path.join(original_dir, 'sokol.inl')

        print(f'Generating sokol.inl...')
        print(f'Output: {output_path}')
        print()

        # Run generator
        gen_cpp.prepare()

        for task in tasks:
            [c_header_path, main_prefix, dep_prefixes] = task
            try:
                gen_cpp.gen(c_header_path, main_prefix, dep_prefixes)
            except Exception as e:
                print(f'  >> warning: skipped {main_prefix} due to error: {str(e)[:100]}')
                import traceback
                traceback.print_exc()

        gen_cpp.finalize(output_path)

        print()
        print(f'Successfully generated {output_path}')

        # Show stats
        with open(output_path, 'r') as f:
            lines = len(f.readlines())
        print(f'Generated {lines} lines of code')

    finally:
        # Restore original directory
        os.chdir(original_dir)

if __name__ == '__main__':
    main()
