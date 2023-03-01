import vtk

inFileName = '1aua/1aua.wrl'
outFilePrefix = '1aua/1aua'

reader = vtk.vtkVRMLImporter()
reader.SetFileName(inFileName)

renderer = vtk.vtkRenderer()

window = vtk.vtkRenderWindow()
window.AddRenderer(renderer)

reader.SetRenderWindow(window)
reader.Read()

print 'Reading ' + inFileName

window.Render()

exporter = vtk.vtkOBJExporter()
exporter.SetFilePrefix(outFilePrefix)
exporter.SetRenderWindow(window)
exporter.Write()

print 'Writing ' + outFilePrefix + '.obj'

#exporter2 = vtk.vtkVRMLExporter()
#exporter2.SetFileName('sfh1_PC_PI/balls.wrl')
#exporter2.SetRenderWindow(window)
#exporter2.Write()