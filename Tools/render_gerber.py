# -*- coding: utf-8 -*-
import argparse
from pathlib import Path

from pygerber.gerberx3.api.v2 import FileTypeEnum, GerberFile


REPO_ROOT = Path(__file__).resolve().parent.parent
GERBER_DIR = REPO_ROOT / 'Hardware' / 'PCB'

# 渲染顶层铜层
files_to_render = [
    ('Gerber_TopLayer.GTL', 'PCB_TopLayer.png'),
    ('Gerber_BottomLayer.GBL', 'PCB_BottomLayer.png'),
    ('Gerber_TopSilkscreenLayer.GTO', 'PCB_Silkscreen.png'),
]

def main():
    parser = argparse.ArgumentParser(description='Render Gerber layers to PNG files.')
    parser.add_argument(
        '--output-dir',
        type=Path,
        default=REPO_ROOT,
        help='Output directory (default: repository root)',
    )
    args = parser.parse_args()
    args.output_dir.mkdir(parents=True, exist_ok=True)

    for gerber_name, out_name in files_to_render:
        path = GERBER_DIR / gerber_name
        if path.exists():
            print(f'Rendering {gerber_name}...')
            parsed = GerberFile.from_file(
                path,
                file_type=FileTypeEnum.INFER,
            ).parse()
            out_path = args.output_dir / out_name
            parsed.render_raster(out_path, dpmm=8)
            print(f'  -> {out_path}')
        else:
            print(f'File not found: {path}')

    print('Rendering complete')


if __name__ == '__main__':
    main()
