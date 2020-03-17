describe('display', () => {
  it('should work with inline-block', async () => {

    const container = document.createElement('div');
    setStyle(container, {
      width: '100px',
      height: '100px',
      display: 'inline-block',
      backgroundColor: '#666',
    });

    document.body.appendChild(container);
    document.body.appendChild(document.createTextNode('This text should display as the same line as the box'));

    await expectAsync(document.body.toBlob(1)).toMatchImageSnapshot();
  });
});
