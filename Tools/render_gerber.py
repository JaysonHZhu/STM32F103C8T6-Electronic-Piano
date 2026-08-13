# -*- coding: utf-8 -*-
import os
from pygerber.gerber.api import GerberFile

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
gerber_dir = os.path.join(REPO_ROOT, 'Hardware', 'PCB')
output_dir = REPO_ROOT

# 渲染顶层铜层
files_to_render = [
    ('Gerber_TopLayer.GTL', 'PCB_TopLayer.png'),
    ('Gerber_BottomLayer.GBL', 'PCB_BottomLayer.png'),
    ('Gerber_TopSilkscreenLayer.GTO', 'PCB_Silkscreen.png'),
]

for gerber_name, out_name in files_to_render:
    path = os.path.join(gerber_dir, gerber_name)
    if os.path.exists(path):
        print(f'正在渲染 {gerber_name}...')
        gf = GerberFile.from_file(path)
        out_path = os.path.join(output_dir, out_name)
        gf.render_rasterized(out_path, dpi=200)
        print(f'  -> {out_path}')
    else:
        print(f'文件不存在: {path}')

print('渲染完成')
