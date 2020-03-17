describe('display', () => {
  it('should work with inline-flex', async () => {

    const container = document.createElement('div');
    setStyle(container, {
      width: '100px',
      height: '100px',
      display: 'inline-flex',
      justifyContent: 'center',
      alignItems: 'center',
      backgroundColor: '#666',
    });
    container.appendChild(document.createTextNode('inline-flex'));

    document.body.appendChild(container);
    document.body.appendChild(document.createTextNode('This text should display as the same line as the box'));

    await expectAsync(document.body.toBlob(1)).toMatchImageSnapshot();
  });
});
